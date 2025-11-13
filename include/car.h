#ifndef CAR_H
#define CAR_H
#include <string>
#include <chrono>

using namespace std;

extern double mass_kg; // 1500 kg
// SW
extern const int sw_angle_min;
extern const int sw_angle_max;
extern int sw_angle;
extern float wheel_angle;

// Turn indicator 
extern string turn;
extern bool flashing_r;
extern bool flashing_l;
extern bool show_symbol;
extern std::chrono::steady_clock::time_point last_flash_time;

#endif