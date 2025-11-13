#include "engine.h"

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distr1(800, 900); // Random for idle.
uniform_int_distribution<> distr2(50, 100); // Random for rpm.
int rpm = 0;
double max_hp = 200.0; // 200KM

int min_rpm = 0;
int max_rpm = 6500;
auto last_update_time = std::chrono::steady_clock::now();
bool increase_rpm = false;
bool decrease_rpm = false;

bool engine = false;