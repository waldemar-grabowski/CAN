#include <ncurses.h>
#include <thread>
#include <chrono>

using namespace std;

int kmh = 0;
const int min_kmh = 0;
const int max_kmh = 200;

const int sw_angle_min = -540;
const int sw_angle_max = 540;
int sw_angle = 0;

float wheel_angle = 0;

char key;

string turn;
bool flashing_r = false;
bool flashing_l = false;
bool show_symbol = false;
auto last_flash_time = std::chrono::steady_clock::now();

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

float wheels() {
    wheel_angle = sw_angle / 36; // Przeliczanie polozenia kierwonicy na, polozenie kol.

    return wheel_angle;
}

int steering() {
    // Za kazdym wcisnieciem D, skrec kierownice w prawo o 10 stopni.
    if (key == 'd') {
        sw_angle += 10;
        if (sw_angle > sw_angle_max) {
            sw_angle = sw_angle_max;
        }
    } else if (key == 'a') {
        sw_angle -= 10;
    } if (sw_angle < sw_angle_min) {
        sw_angle = sw_angle_min;
    }
    // Za kazdym wcisnieciem A, skrec kierownice w lewo o 10 stopni.

    return sw_angle;
}

string turn_indicator() {
    if (key == 'q') {
        flashing_l = !flashing_l;
        // Wyłącz prawą migającą strzałkę, jeśli właczasz lewą (opcjonalnie)
        if (flashing_l) flashing_r = false;
    } else if (key == 'e') {
        flashing_r = !flashing_r;
        // Wyłącz lewą, jeśli właczasz prawą (opcjonalnie)
        if (flashing_r) flashing_l = false;
    }

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_flash_time).count();

    if (duration > 1000) {
        if (flashing_l || flashing_r) {
            show_symbol = !show_symbol; // Przełącz miganie (pokaz/ukryj)
        } else {
            show_symbol = false; // jeśli wyłączone, nie pokazuj nic
        }
        last_flash_time = now;
    }

    if (flashing_l && show_symbol) {
        turn = "<-";
    } else if (flashing_r && show_symbol) {
        turn = "->";
    } else {
        turn = " ";
    }


    return turn;
}


int main(){
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);

    while (true) {
        clear();
        
        key = getch();
        if (key == 'w' or key == 's') speed();
        if (key == 'a' or key == 'd') steering();
        turn = turn_indicator();
        wheel_angle = wheels();

        printw("Your speed is: %d km/h\nSteering wheel angle is: %d°\nWheels angle is: %.1f°\nTurn indicator: %s", kmh, sw_angle, wheel_angle, turn.c_str());
        refresh();

        napms(10);
    }
    

    endwin();
    return 0;
}