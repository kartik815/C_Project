#include "globals.h"
#include <Wire.h>
#include <MPU6050.h>

// Number of packets to send in each array
#define ARRAY_SIZE 5


Data_packet readingsBuffer[ARRAY_SIZE];
int readingIndex = 0;
      // replace with correct port for this glove

void Send_Array(Data_packet* packets, int n) {
    udp.beginPacket(laptopIP, laptopPort);
    udp.write((uint8_t*)packets, n * sizeof(Data_packet));
    udp.endPacket();
}

void setup() {
    
    pinMode(btn_pin1, INPUT_PULLUP);
    pinMode( btn_pin2, INPUT_PULLUP);

    Serial.begin(115200);
    // Connect ESP32 to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Start UDP on a local port
    udp.begin(laptopPort);

     Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  } 

  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);

  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);

  Serial.println("Calibrating...");
  last_time = micros();
  initialize_angle_values();
  
  Serial.println("Ready.");

}

void loop() {
   
    for (int i = 0; i < ARRAY_SIZE; i++){
      update_angle_values();
      Add_Reading_To_Storage(get_sensor_value());
       Serial.print("Roll: ");
      Serial.print(roll_angle, 2);
      Serial.print("  Pitch: ");
      Serial.println(pitch_angle, 2);
      Serial.print("Code:" );
      Serial.println(get_sensor_value().code);
      delay(50);
    }

    // Send array to laptop
    Send_Array(readingsBuffer, ARRAY_SIZE);
    delay(10);
    
}




//Function 2:Add_Reading_To_Storage()

void Add_Reading_To_Storage(Data_packet newPacket) {
    readingsBuffer[readingIndex] = newPacket;
    readingIndex++;
    if (readingIndex >= ARRAY_SIZE) {
        readingIndex = 0;  // wrap around (circular buffer)
    }
}

