#include "sensors/SPIDeviceManager.h"
#include <Arduino.h>

void SPIDeviceManager::begin(int sckPin, int misoPin, int mosiPin, int csPin, uint32_t clockSpeed) {
    SPI.begin(sckPin, misoPin, mosiPin, csPin);
    SPI.setFrequency(clockSpeed);
    Serial.println("SPI Bus initialized");
}

uint8_t SPIDeviceManager::readRegister(int csPin, uint8_t reg) {
    beginTransaction(csPin);
    SPI.transfer(reg | 0x80); // Read bit
    uint8_t value = SPI.transfer(0x00);
    endTransaction(csPin);
    return value;
}

bool SPIDeviceManager::writeRegister(int csPin, uint8_t reg, uint8_t value) {
    beginTransaction(csPin);
    SPI.transfer(reg & 0x7F); // Write bit
    SPI.transfer(value);
    endTransaction(csPin);
    return true;
}

bool SPIDeviceManager::readRegisters(int csPin, uint8_t reg, uint8_t* data, size_t length) {
    beginTransaction(csPin);
    SPI.transfer(reg | 0x80); // Read bit
    for (size_t i = 0; i < length; i++) {
        data[i] = SPI.transfer(0x00);
    }
    endTransaction(csPin);
    return true;
}

bool SPIDeviceManager::writeRegisters(int csPin, uint8_t reg, const uint8_t* data, size_t length) {
    beginTransaction(csPin);
    SPI.transfer(reg & 0x7F); // Write bit
    for (size_t i = 0; i < length; i++) {
        SPI.transfer(data[i]);
    }
    endTransaction(csPin);
    return true;
}

void SPIDeviceManager::beginTransaction(int csPin) {
    digitalWrite(csPin, LOW);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void SPIDeviceManager::endTransaction(int csPin) {
    SPI.endTransaction();
    digitalWrite(csPin, HIGH);
} 