#ifndef MPU6500_H
#define MPU6500_H

#include "I2CDeviceManager.h"
#include "SPIDeviceManager.h"

enum class MPU6500Interface {
    I2C,
    SPI
};

class MPU6500 {
public:
    // I2C constructor
    MPU6500(uint8_t address = 0x68);
    
    // SPI constructor
    MPU6500(int csPin, MPU6500Interface interface = MPU6500Interface::SPI);
    
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
    
private:
    MPU6500Interface interface;
    uint8_t i2cAddress;
    int spiCsPin;
    int16_t accelX, accelY, accelZ;
    int16_t gyroX, gyroY, gyroZ;
    int16_t temperature;
    
    // Interface-specific methods
    uint8_t readRegister(uint8_t reg);
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegisters(uint8_t reg, uint8_t* data, size_t length);
};

#endif 