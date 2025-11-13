#include "gearbox.h"

double wheel_radius = 0.3;     // w metrach
double final_drive = 3.91;
int min_gear = 0;
int max_gear = 6;
double gear_ratios[] = {10000000.0, 3.6, 2.1, 1.4, 1.0, 0.83, 0.72};

const int min_kmh = 0; 
const int max_kmh = 200;


int gear = 0;
int kmh = 0;