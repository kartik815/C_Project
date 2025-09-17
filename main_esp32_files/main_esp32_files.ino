#include <Wire.h>
#include <MPU6050.h>
#include "globals.h"

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  Serial.println("Calibrating...");
  last_time = micros();
  initialize_angle_values();
  
  Serial.println("Ready.");
}

// ------------------------------------------------------------
void loop() {
  update_angle_values();

  Serial.print("Roll: ");
  Serial.print(roll_angle, 2);
  Serial.print("  Pitch: ");
  Serial.println(pitch_angle, 2);

  delay(20); // ~50 Hz update
}