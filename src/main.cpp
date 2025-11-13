#include <ncurses.h>
#include <thread>
#include <chrono>
#include <random>
#include "engine.h"
#include "gearbox.h"
#include "car.h"

using namespace std;

char key;

int speed() {
    // Za kazdym wcisnieciem W, zwieksz predkosc o 1.
    if (key == 'w') {
        kmh += 1;
        if (kmh > max_kmh) {
            kmh = max_kmh;
        }
    } else if (key == 's') {
        kmh -= 10;
        if (kmh < min_kmh) {
            kmh = min_kmh;
        }
    }
    // Za kazdym wcisnieciem S, zmniejsz predkosc o 2.
    return kmh;
}

float wheels() {
    wheel_angle = sw_angle / 36; // Przeliczanie polozenia kierwonicy na, polozenie kol.

    return wheel_angle;
}

int steering() {
    // Za kazdym wcisnieciem D, skrec kierownice w prawo o 10 stopni.
    if (key == 'd') {
        sw_angle += 5;
        if (sw_angle > sw_angle_max) {
            sw_angle = sw_angle_max;
        }
    } else if (key == 'a') {
        sw_angle -= 5;
    } if (sw_angle < sw_angle_min) {
        sw_angle = sw_angle_min;
    }
    // Za kazdym wcisnieciem A, skrec kierownice w lewo o 10 stopni.

    return sw_angle;
}

string turn_indicator() {    
    if (key == 'q') {
        flashing_l = !flashing_l;
        if (flashing_l) flashing_r = false;
    } else if (key == 'e') {
        flashing_r = !flashing_r;
        if (flashing_r) flashing_l = false;
    }

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_flash_time).count();

    if (duration > 1000) {
        if (flashing_l || flashing_r) {
            show_symbol = !show_symbol; 
        } else {
            show_symbol = false; 
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



int engine_rpm() {
    static auto last_update_time = std::chrono::steady_clock::now();
    static int rpm = 0;
    
    if (key == 'w' and engine) {
    increase_rpm = true;
    decrease_rpm = false;
    } else if (!engine) {
    decrease_rpm = false;
    increase_rpm = false;
    rpm = 0;
    } else {
        decrease_rpm = true;
        increase_rpm = false;
    }

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time).count();

    if (increase_rpm && duration > 10) {
            rpm += distr2(gen);
            last_update_time = now;
            if (rpm > max_rpm) rpm = max_rpm;
        } else if (decrease_rpm && duration > 100) {
            rpm -= distr2(gen);
            last_update_time = now;
            if (rpm < distr1(gen)) rpm = distr1(gen);
        }

    return rpm;
}

int gear_shift() {
    if (key == 'p') {
        gear += 1;
    } else if (key == 'l') {
        gear -= 1;
    }
    
    /* if (kmh <= 20){
        gear = 1;
    } else if (kmh > 20 and kmh <= 50 and rpm > 3500){
        gear = 2;
    } else if (kmh > 50 and kmh <= 70 and rpm > 3500){
        gear = 3;
    } else if (kmh > 70 and kmh <= 100 and rpm > 3500){
        gear = 4;
    } else if (kmh > 100 and kmh <= 140 and rpm > 3500){
        gear = 5;
    } else if (kmh > 140){
        gear = 6;
    } */

    return gear;
}

bool engine_running() {
    if (key == 'i') {
        engine = !engine;
    }

    return engine;
}

double horsepower_to_watts(double hp){ // Napisz to swoim jezykiem tak zeby zrozumiec.
    return hp * 735.5;
}

double engine_power_at_rpm(int rpm, int max_rpm, double max_hp){ // Napisz to swoim jezykiem tak zeby zrozumiec.
    return max_hp * (double)rpm / max_rpm;
}

double acceleration(double power_hp, double velocity_m_s, double mass_kg) { // Napisz to swoim jezykiem tak zeby zrozumiec.
    double power_w = horsepower_to_watts(power_hp);
    if (velocity_m_s < 0.1) velocity_m_s = 0.1;
    double force = power_w / velocity_m_s;
    return force / mass_kg;
}


int main(){
    int rpm;
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);

    while (true) {
        clear();
        
        key = getch();
        if (key == ERR) {
        key = '\0'; // no key pressed
        }
        if (key == 's') speed();
        if (key == 'a' or key == 'd') steering();
        if (key == 'i') engine_running(); // Turn ON or OFF the ignition.
        turn = turn_indicator();
        wheel_angle = wheels();
        gear = gear_shift();
        rpm = engine_rpm();
        double wheel_rpm = rpm / (gear_ratios[gear] * final_drive); // Napisz to swoim jezykiem tak zeby zrozumiec.
        double wheel_speed_m_per_min = wheel_rpm * 2 * M_PI * wheel_radius; // Napisz to swoim jezykiem tak zeby zrozumiec.
        double velocity_kmh = (wheel_speed_m_per_min / 1000.0) * 60.0; // Napisz to swoim jezykiem tak zeby zrozumiec.
        kmh = velocity_kmh; // Napisz to swoim jezykiem tak zeby zrozumiec.
        

        printw("Your speed is: %d km/h\nSteering wheel angle is: %d°\nWheels angle is: %.1f°\nTurn indicator: %s\nGear: %d\nRpm: %d\nEngine running: %s", kmh, sw_angle, wheel_angle, turn.c_str(), gear, rpm, engine ? "true" : "false"); // engine ? -> Przypisz string do true i false. 
        refresh();

        napms(20); // Refresh time for the window
    }
    

    endwin();
    return 0;
}