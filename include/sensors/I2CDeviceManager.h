#ifndef I2CDEVICEMANAGER_H
#define I2CDEVICEMANAGER_H

#include <Wire.h>

class I2CDeviceManager {
public:
    // Initialize I2C bus
    static void begin(int sdaPin = 21, int sclPin = 22, uint32_t clockSpeed = 400000);
    
    // Scan and list all I2C devices
    static void scan();
    
    // Check if device exists at address
    static bool deviceExists(uint8_t address);
    
    // Read/Write operations
    static uint8_t readRegister(uint8_t address, uint8_t reg);
    static bool writeRegister(uint8_t address, uint8_t reg, uint8_t value);
    static bool readRegisters(uint8_t address, uint8_t reg, uint8_t* data, size_t length);
    static bool writeRegisters(uint8_t address, uint8_t reg, const uint8_t* data, size_t length);
};

#endif 