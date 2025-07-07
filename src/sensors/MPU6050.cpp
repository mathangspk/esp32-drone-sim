#include "sensors/MPU6050.h"

MPU6050::MPU6050(uint8_t addr) : address(addr) {}

bool MPU6050::init() {
    // Check if device exists
    if (!I2CDeviceManager::deviceExists(address)) {
        return false;
    }
    
    // Check WHO_AM_I register
    if (I2CDeviceManager::readRegister(address, 0x75) != 0x68) {
        return false;
    }
    
    // Wake up device
    I2CDeviceManager::writeRegister(address, 0x6B, 0x00);
    
    // Configure gyroscope (±2000°/s)
    I2CDeviceManager::writeRegister(address, 0x1B, 0x18);
    
    // Configure accelerometer (±2g)
    I2CDeviceManager::writeRegister(address, 0x1C, 0x00);
    
    return true;
}

void MPU6050::read() {
    uint8_t data[14];
    if (I2CDeviceManager::readRegisters(address, 0x3B, data, 14)) {
        accelX = (data[0] << 8) | data[1];
        accelY = (data[2] << 8) | data[3];
        accelZ = (data[4] << 8) | data[5];
        temperature = (data[6] << 8) | data[7];
        gyroX = (data[8] << 8) | data[9];
        gyroY = (data[10] << 8) | data[11];
        gyroZ = (data[12] << 8) | data[13];
    }
} 