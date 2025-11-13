#include "car.h"

double mass_kg = 1500.0; // 1500 kg
// SW
const int sw_angle_min = -540;
const int sw_angle_max = 540;
int sw_angle = 0;
float wheel_angle = 0;

// Turn indicator 
string turn;
bool flashing_r = false;
bool flashing_l = false;
bool show_symbol = false;
auto last_flash_time = std::chrono::steady_clock::now();