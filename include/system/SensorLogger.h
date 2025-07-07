#ifndef SENSORLOGGER_H
#define SENSORLOGGER_H

#include "sensors/MPU6050.h"
#include "sensors/MPU6500.h"
#include "sensors/BME280.h"
#include "sensors/QMC5883L.h"
#include "system/SerialCommandHandler.h"
#include "config.h"

class SensorLogger {
public:
    SensorLogger(SerialCommandHandler& serial);

    void logAllSensors(
        #if USE_MPU6050
        const MPU6050& mpu6050,
        #endif
        #if USE_MPU6500
        const MPU6500& mpu6500,
        #endif
        #if USE_BME280
        const BME280& bme,
        #endif
        #if USE_QMC5883L
        const QMC5883L& compass,
        #endif
        float roll_deg, float pitch_deg, float yaw_deg,
        float gyroX_dps, float gyroY_dps, float gyroZ_dps,
        unsigned long timestamp_ms
    );

private:
    SerialCommandHandler& serial;
};

#endif