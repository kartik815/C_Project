#ifndef GLOBALS_H
#define GLOBALS_H

#include <MPU6050.h>

// Declare global variables (not define them here)
extern MPU6050 mpu;
extern unsigned long now, last_time;
extern float roll_angle, pitch_angle;
extern const uint8_t btn_pin1, btn_pin2;

struct Data_packet{
  unsigned long timestamp;
  uint8_t code;
};
#endif
