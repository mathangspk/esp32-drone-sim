#ifndef IMUDATA_H
#define IMUDATA_H

#include "sensors/MPU6050.h"
#include "sensors/MPU6500.h"
#include "sensors/BME280.h"
#include "sensors/QMC5883L.h"

struct SensorData {
    MPU6050 mpu6050;
    MPU6500 mpu6500;
    BME280 bme280;
    QMC5883L compass;
    float roll_deg, pitch_deg, yaw_deg;
    float gyroX_dps, gyroY_dps, gyroZ_dps;
    uint16_t throttle, roll, pitch, yaw, aux1, aux2;

    unsigned long timestamp_ms;
};

#endif