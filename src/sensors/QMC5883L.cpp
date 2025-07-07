#include "sensors/QMC5883L.h"
#include <math.h>

QMC5883L::QMC5883L(uint8_t addr) : address(addr) {}

bool QMC5883L::init() {
    // Check if device exists
    if (!I2CDeviceManager::deviceExists(address)) {
        return false;
    }
    
    // Check chip ID
    if (I2CDeviceManager::readRegister(address, 0x0D) != 0xFF) {
        return false;
    }
    
    // Configure sensor
    I2CDeviceManager::writeRegister(address, 0x09, 0x0D); // Set/Reset period
    I2CDeviceManager::writeRegister(address, 0x0A, 0x80); // Control register 1
    I2CDeviceManager::writeRegister(address, 0x0B, 0x01); // Control register 2
    
    return true;
}

void QMC5883L::read() {
    uint8_t data[6];
    if (I2CDeviceManager::readRegisters(address, 0x00, data, 6)) {
        x = (int16_t)(data[1] << 8) | data[0];
        y = (int16_t)(data[3] << 8) | data[2];
        z = (int16_t)(data[5] << 8) | data[4];
    }
}

float QMC5883L::getHeading() const {
    float heading = atan2(y, x);
    
    // Convert to degrees
    heading = heading * 180.0 / M_PI;
    
    // Normalize to 0-360
    if (heading < 0) {
        heading += 360.0;
    }
    
    return heading;
} 