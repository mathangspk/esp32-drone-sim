#include "sensors/I2CDeviceManager.h"
#include <Arduino.h>

void I2CDeviceManager::begin(int sdaPin, int sclPin, uint32_t clockSpeed) {
    Wire.begin(sdaPin, sclPin);
    Wire.setClock(clockSpeed);
    Serial.println("I2C Bus initialized");
}

void I2CDeviceManager::scan() {
    Serial.println("I2C Device Scan:");
    Serial.println("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
    
    for (uint8_t address = 0; address < 128; address++) {
        if (address % 16 == 0) {
            Serial.println();
            Serial.print(address / 16, HEX);
            Serial.print("0 ");
        }
        
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print(" ");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
        } else {
            Serial.print(" --");
        }
    }
    Serial.println();
}

bool I2CDeviceManager::deviceExists(uint8_t address) {
    Wire.beginTransmission(address);
    return Wire.endTransmission() == 0;
}

uint8_t I2CDeviceManager::readRegister(uint8_t address, uint8_t reg) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(address, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;
}

bool I2CDeviceManager::writeRegister(uint8_t address, uint8_t reg, uint8_t value) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}

bool I2CDeviceManager::readRegisters(uint8_t address, uint8_t reg, uint8_t* data, size_t length) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(address, (uint8_t)length);
    
    if (Wire.available() >= length) {
        for (size_t i = 0; i < length; i++) {
            data[i] = Wire.read();
        }
        return true;
    }
    return false;
}

bool I2CDeviceManager::writeRegisters(uint8_t address, uint8_t reg, const uint8_t* data, size_t length) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    for (size_t i = 0; i < length; i++) {
        Wire.write(data[i]);
    }
    return Wire.endTransmission() == 0;
} 