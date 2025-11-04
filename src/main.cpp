#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

struct termios orig_termios;

int kmh = 0;
const int min_kmh = 0;
const int max_kmh = 200;
char key;

void set_raw_mode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

char take_user_input(){
    read(STDIN_FILENO, &key, 1);

    return key;
}

void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

int speed() {
    // Za kazdym wcisnieciem W, zwieksz predkosc o 10.
    if (key == 'w') {
        kmh += 10;
        if (kmh > max_kmh) {
            kmh = max_kmh;
        }
    } else if (key == 's') {
        kmh -= 20;
        if (kmh < min_kmh) {
            kmh = min_kmh;
        }
    }
    // Za kazdym wcisnieciem S, zmniejsz predkosc o 20.
    return kmh;
} 




int main(){
    set_raw_mode();
    do {
        key = take_user_input();
        cout << "\033[2J\033[H"; // ANSI Escape, czysci terminal.
        kmh = speed();
        cout << "Your speed is: " << kmh << endl;
    } while (key != ('.'));
    reset_terminal();

    return 0;
}