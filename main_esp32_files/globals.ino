#include "globals.h"

// Define the global variables here (exactly once)
MPU6050 mpu;
unsigned long now = 0, last_time = 0;
float roll_angle = 0.0;
float pitch_angle = 0.0;
