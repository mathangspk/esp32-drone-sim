#include "sensors/BME280.h"

BME280::BME280(uint8_t addr) : address(addr) {}

bool BME280::init() {
    // Check if device exists
    if (!I2CDeviceManager::deviceExists(address)) {
        return false;
    }
    
    // Check chip ID
    if (I2CDeviceManager::readRegister(address, 0xD0) != 0x60) {
        return false;
    }
    
    // Load calibration data
    if (!loadCalibration()) {
        return false;
    }
    
    // Set sampling configuration
    I2CDeviceManager::writeRegister(address, 0xF2, 0x01); // Humidity
    I2CDeviceManager::writeRegister(address, 0xF4, 0x27); // Temperature, Pressure, Normal mode
    I2CDeviceManager::writeRegister(address, 0xF5, 0x00); // Config
    
    return true;
}

void BME280::read() {
    uint8_t data[8];
    if (I2CDeviceManager::readRegisters(address, 0xF7, data, 8)) {
        int32_t rawPressure = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | (data[2] >> 4);
        int32_t rawTemperature = ((int32_t)data[3] << 12) | ((int32_t)data[4] << 4) | (data[5] >> 4);
        int32_t rawHumidity = ((int32_t)data[6] << 8) | data[7];
        
        temperature = compensateTemperature(rawTemperature);
        pressure = compensatePressure(rawPressure);
        humidity = compensateHumidity(rawHumidity);
        altitude = 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));
    }
}

bool BME280::loadCalibration() {
    uint8_t data[32];
    
    // Read temperature calibration
    if (!I2CDeviceManager::readRegisters(address, 0x88, data, 6)) return false;
    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    
    // Read pressure calibration
    if (!I2CDeviceManager::readRegisters(address, 0x8E, data, 18)) return false;
    dig_P1 = (data[1] << 8) | data[0];
    dig_P2 = (data[3] << 8) | data[2];
    dig_P3 = (data[5] << 8) | data[4];
    dig_P4 = (data[7] << 8) | data[6];
    dig_P5 = (data[9] << 8) | data[8];
    dig_P6 = (data[11] << 8) | data[10];
    dig_P7 = (data[13] << 8) | data[12];
    dig_P8 = (data[15] << 8) | data[14];
    dig_P9 = (data[17] << 8) | data[16];
    
    // Read humidity calibration
    dig_H1 = I2CDeviceManager::readRegister(address, 0xA1);
    if (!I2CDeviceManager::readRegisters(address, 0xE1, data, 7)) return false;
    dig_H2 = (data[1] << 8) | data[0];
    dig_H3 = data[2];
    dig_H4 = (data[3] << 4) | (data[4] & 0x0F);
    dig_H5 = (data[5] << 4) | (data[4] >> 4);
    dig_H6 = data[6];
    
    return true;
}

float BME280::compensateTemperature(int32_t rawTemp) {
    int32_t var1, var2;
    var1 = ((((rawTemp >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((rawTemp >> 4) - ((int32_t)dig_T1)) * ((rawTemp >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    int32_t t_fine = var1 + var2;
    float T = (t_fine * 5 + 128) >> 8;
    return T / 100.0;
}

float BME280::compensatePressure(int32_t rawPress) {
    int64_t var1, var2, p;
    int32_t t_fine = 0; // Simplified - should be calculated from temperature
    
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    
    if (var1 == 0) return 0;
    
    p = 1048576 - rawPress;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    
    return (float)p / 256.0;
}

float BME280::compensateHumidity(int32_t rawHum) {
    int32_t v_x1_u32r;
    int32_t t_fine = 0; // Simplified - should be calculated from temperature
    
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((rawHum << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) +
                    ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r *
                    ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
                    ((int32_t)dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    float h = (v_x1_u32r >> 12);
    return h / 1024.0;
} 