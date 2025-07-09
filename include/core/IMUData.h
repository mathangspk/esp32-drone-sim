#ifndef IMUDATA_H
#define IMUDATA_H

#include "sensors/MPU6050.h"
#include "sensors/MPU6500.h"
#include "sensors/BME280.h"

struct SensorData {
    MPU6050 mpu6050;
    MPU6500 mpu6500;
    BME280 bme280;
    float roll_deg, pitch_deg, yaw_deg;
    float gyroX_dps, gyroY_dps, gyroZ_dps;
    // float heading_deg; // (nếu muốn lưu heading từ compass)
    uint16_t throttle, roll, pitch, yaw, aux1, aux2;

    unsigned long timestamp_ms;
};

#endif