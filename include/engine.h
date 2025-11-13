#ifndef ENGINE_H
#define ENGINE_H
#include <random>

using namespace std;

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_int_distribution<> distr1;
extern std::uniform_int_distribution<> distr2;
extern int rpm;
extern double max_hp; // 200KM

extern int min_rpm;
extern int max_rpm;
extern std::chrono::steady_clock::time_point last_update_time;
extern bool increase_rpm;
extern bool decrease_rpm;

extern bool engine;

#endif