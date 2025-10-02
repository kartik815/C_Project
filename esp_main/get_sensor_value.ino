#include <Wire.h>
#include <MPU6050.h>
#include "globals.h"


int16_t ax,ay, az, gx, gy, gz;

const float ACCEL_SCALE = 8192.0;
const float GYRO_SCALE = 32.8; 
const float GRAVITY = 9.80665; // m/s^2 per g
const float FILTER_COEFFICIENT = 0.95;

// Tolerance angle at which we say that input is triggered (in degrees);
const float ROLL_TOLERANCE = 30; 
const float PITCH_TOLERANCE = 30; 

// offset incase if sensor is always detecting value with an offset
const float ROLL_OFFSET = 0;
const float PITCH_OFFSET = -5;

void initialize_angle_values(){

    float roll = 0;
    float pitch = 0;
    
    for (int i = 0; i < 100; i++){
        // Getting the accelerometer values;
        mpu.getAcceleration(&ax,&ay,&az);
        
        // convert accelerometer → m/s²
        float ax_ms2 = (ax / ACCEL_SCALE) * GRAVITY;
        float ay_ms2 = (ay / ACCEL_SCALE) * GRAVITY;
        float az_ms2 = (az / ACCEL_SCALE) * GRAVITY;

        roll+= atan2(ay_ms2,sqrt(ax_ms2*ax_ms2 + az_ms2*az_ms2))*180.0/M_PI;
        pitch+= atan2(-ax_ms2,sqrt(ay_ms2*ay_ms2 + az_ms2*az_ms2))*180.0/M_PI;
        delay(50);
    }

    pitch_angle = pitch/100.0;
    roll_angle = roll/100.0;
}

void update_angle_values(){
    
    // Getting the data from accelerometer data.
    mpu.getAcceleration(&ax,&ay,&az);
        
    // convert accelerometer → m/s²
    float ax_ms2 = (ax / ACCEL_SCALE) * GRAVITY;
    float ay_ms2 = (ay / ACCEL_SCALE) * GRAVITY;
    float az_ms2 = (az / ACCEL_SCALE) * GRAVITY;

    // Converting the accelerometer data to roll and pitch.
    float acc_roll = atan2(ay_ms2,sqrt(ax_ms2*ax_ms2 + az_ms2*az_ms2))*180.0/M_PI;
    float acc_pitch = atan2(-ax_ms2,sqrt(ay_ms2*ay_ms2 + az_ms2*az_ms2))*180.0/M_PI;

    // Getting the time it is taken to between last reading and now.
    now = micros(); // Getting the time at now from which the board is started.
    float time_taken = (now-last_time)/1e6;
    last_time = now;

    // Getting the gyro sensor values.
    mpu.getRotation(&gx,&gy,&gz);
    float gyro_roll = gx/GYRO_SCALE;
    float gyro_pitch = gy/GYRO_SCALE;

    

    // Adding the roll and pitch angle from gyro sensor.
    roll_angle += gyro_roll*time_taken;
    pitch_angle += gyro_pitch*time_taken;



    // Implementing complementary filter.
    roll_angle = FILTER_COEFFICIENT * roll_angle + (1-FILTER_COEFFICIENT)*acc_roll;
    pitch_angle = FILTER_COEFFICIENT * pitch_angle + (1-FILTER_COEFFICIENT)*acc_pitch;    
}

struct Data_packet get_sensor_value(){
    struct Data_packet data = {0,0};

    data.timestamp = micros();

    if (roll_angle+ROLL_OFFSET > ROLL_TOLERANCE){
        data.code = data.code | 1;
    }    

    if (roll_angle+ROLL_OFFSET < -ROLL_TOLERANCE){
        data.code = data.code | 2;
    }

    if (pitch_angle+PITCH_OFFSET > PITCH_TOLERANCE){
        data.code = data.code | 4;
    }

    if (pitch_angle+PITCH_OFFSET < -PITCH_TOLERANCE){
        data.code = data.code | 8;
    }

    if (digitalRead(btn_pin1) == LOW){
        data.code = data.code | 16;
    }

    if (digitalRead(btn_pin2) == LOW){
        data.code = data.code | 32;
    }

    data.code = data.code | 64;
    return data;
}

