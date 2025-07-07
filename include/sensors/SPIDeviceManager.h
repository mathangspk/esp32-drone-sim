#ifndef SPIDEVICEMANAGER_H
#define SPIDEVICEMANAGER_H

#include <SPI.h>

class SPIDeviceManager {
public:
    // Initialize SPI bus
    static void begin(int sckPin = 18, int misoPin = 19, int mosiPin = 23, int csPin = 5, uint32_t clockSpeed = 1000000);
    
    // Read/Write operations
    static uint8_t readRegister(int csPin, uint8_t reg);
    static bool writeRegister(int csPin, uint8_t reg, uint8_t value);
    static bool readRegisters(int csPin, uint8_t reg, uint8_t* data, size_t length);
    static bool writeRegisters(int csPin, uint8_t reg, const uint8_t* data, size_t length);
    
    // SPI transaction helpers
    static void beginTransaction(int csPin);
    static void endTransaction(int csPin);
};

#endif 