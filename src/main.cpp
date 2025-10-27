#include <iostream>
#include <termios.h>
#include <unistd.h>
using namespace std;

struct termios orig_termios;

void set_raw_mode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

char take_user_input(){
    char key;
    read(STDIN_FILENO, &key, 1);

    return key;
}

void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}




int main(){
    char key;
    set_raw_mode();
    do {
        key = take_user_input();
        cout << "Your key is: " << key << endl;
    } while (key != ('.'));
    reset_terminal();

    return 0;
}