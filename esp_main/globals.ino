#include "globals.h"

// Defining/Initializing them the global variables here.
MPU6050 mpu;
unsigned long now = 0, last_time = 0;
float roll_angle = 0.0;
float pitch_angle = 0.0;
WiFiUDP udp;  // UDP object
const uint8_t btn_pin1 = 18, btn_pin2 = 19;


// WiFi credentials
const char* ssid = "Ram_hotspot";
const char* password = "aaaaaaaa";

// Destination laptop details
const char* laptopIP = "10.45.87.26";
const int laptopPort = 8081;      



