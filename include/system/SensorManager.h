#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "sensors/MPU6050.h"
#include "sensors/MPU6500.h"
#include "sensors/BME280.h"
#include "sensors/I2CDeviceManager.h"
#include "sensors/SPIDeviceManager.h"
#include "system/SerialCommandHandler.h"
#include "core/IMUData.h"
#include "core/IMUFilter.h"
#include "core/ComplementaryFilter.h"
#include "QMC5883LCompass.h"

class SensorManager {
public:
    SensorManager(SerialCommandHandler& serial);
    
    bool initAll();
    void readAll();
    
    // Get sensor references
    MPU6050& getMPU6050() { return mpu6050; }
    MPU6500& getMPU6500() { return mpu6500; }
    BME280& getBME280() { return bme; }
    QMC5883LCompass& getQMC5883L() { return compass; }
    
    // Get attitude (roll, pitch, yaw)
    const ComplementaryFilter& getAttitude() const { return attitude; }

private:
    SerialCommandHandler& serial;
    MPU6050 mpu6050;  // I2C
    MPU6500 mpu6500;  // SPI
    BME280 bme;       // I2C
    QMC5883LCompass compass; // I2C

    BiquadLPF gyro6050Filter[3], gyro6500Filter[3];
    BiquadLPF accel6050Filter[3], accel6500Filter[3];
    ComplementaryFilter attitude;

    bool initSensor(const char* name, bool success);
};

#endif