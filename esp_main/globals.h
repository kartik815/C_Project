#ifndef GLOBALS_H
#define GLOBALS_H

#include <MPU6050.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <stdint.h>

// Declare global variables (not define them here)
extern MPU6050 mpu;
extern unsigned long now, last_time;
extern float roll_angle, pitch_angle;
extern const uint8_t btn_pin1, btn_pin2;
extern WiFiUDP udp;
// WiFi credentials
extern const char* ssid;
extern const char* password;

// Destination laptop details
extern const char* laptopIP; // replace with your laptop IP
extern const int laptopPort;

struct Data_packet{
  unsigned long timestamp;
  uint8_t code;
};


#endif
