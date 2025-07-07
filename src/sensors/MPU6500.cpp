#include "sensors/MPU6500.h"

// I2C constructor
MPU6500::MPU6500(uint8_t address) 
    : interface(MPU6500Interface::I2C), i2cAddress(address), spiCsPin(-1) {}

// SPI constructor
MPU6500::MPU6500(int csPin, MPU6500Interface interface) 
    : interface(interface), i2cAddress(0), spiCsPin(csPin) {}

bool MPU6500::init() {
    if (interface == MPU6500Interface::I2C) {
        // Check if device exists
        if (!I2CDeviceManager::deviceExists(i2cAddress)) {
            return false;
        }
        
        // Check WHO_AM_I register
        if (readRegister(0x75) != 0x70) { // MPU6500 WHO_AM_I = 0x70
            return false;
        }
    } else {
        // SPI mode - check WHO_AM_I
        if (readRegister(0x75) != 0x70) {
            return false;
        }
    }
    
    // Wake up device
    writeRegister(0x6B, 0x00);
    
    // Configure gyroscope (±2000°/s)
    writeRegister(0x1B, 0x18);
    
    // Configure accelerometer (±2g)
    writeRegister(0x1C, 0x00);
    
    return true;
}

void MPU6500::read() {
    uint8_t data[14];
    if (readRegisters(0x3B, data, 14)) {
        accelX = (data[0] << 8) | data[1];
        accelY = (data[2] << 8) | data[3];
        accelZ = (data[4] << 8) | data[5];
        temperature = (data[6] << 8) | data[7];
        gyroX = (data[8] << 8) | data[9];
        gyroY = (data[10] << 8) | data[11];
        gyroZ = (data[12] << 8) | data[13];
    }
}

uint8_t MPU6500::readRegister(uint8_t reg) {
    if (interface == MPU6500Interface::I2C) {
        return I2CDeviceManager::readRegister(i2cAddress, reg);
    } else {
        return SPIDeviceManager::readRegister(spiCsPin, reg);
    }
}

bool MPU6500::writeRegister(uint8_t reg, uint8_t value) {
    if (interface == MPU6500Interface::I2C) {
        return I2CDeviceManager::writeRegister(i2cAddress, reg, value);
    } else {
        return SPIDeviceManager::writeRegister(spiCsPin, reg, value);
    }
}

bool MPU6500::readRegisters(uint8_t reg, uint8_t* data, size_t length) {
    if (interface == MPU6500Interface::I2C) {
        return I2CDeviceManager::readRegisters(i2cAddress, reg, data, length);
    } else {
        return SPIDeviceManager::readRegisters(spiCsPin, reg, data, length);
    }
} 