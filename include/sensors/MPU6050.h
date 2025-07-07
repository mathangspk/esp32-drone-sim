#ifndef MPU6050_H
#define MPU6050_H

#include "I2CDeviceManager.h"

// MPU6050 Register Addresses
#define MPU6050_WHO_AM_I        0x75
#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_CONFIG          0x1A
#define MPU6050_GYRO_CONFIG     0x1B
#define MPU6050_ACCEL_CONFIG    0x1C
#define MPU6050_ACCEL_XOUT_H    0x3B

class MPU6050 {
public:
    MPU6050(uint8_t address = 0x68);
    
    bool init();
    void read();
    
    // Get sensor data
    int16_t getAccelX() const { return accelX; }
    int16_t getAccelY() const { return accelY; }
    int16_t getAccelZ() const { return accelZ; }
    int16_t getGyroX() const { return gyroX; }
    int16_t getGyroY() const { return gyroY; }
    int16_t getGyroZ() const { return gyroZ; }
    float getTemperature() const { return temperature / 340.0 + 36.53; }
    
    // Raw data getters
    int16_t getRawAccelX() const { return accelX; }
    int16_t getRawAccelY() const { return accelY; }
    int16_t getRawAccelZ() const { return accelZ; }
    int16_t getRawGyroX() const { return gyroX; }
    int16_t getRawGyroY() const { return gyroY; }
    int16_t getRawGyroZ() const { return gyroZ; }
    int16_t getRawTemperature() const { return temperature; }

private:
    uint8_t address;
    int16_t accelX, accelY, accelZ;
    int16_t gyroX, gyroY, gyroZ;
    int16_t temperature;
};

#endif 