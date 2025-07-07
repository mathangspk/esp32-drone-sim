#ifndef BME280_H
#define BME280_H

#include "I2CDeviceManager.h"

// BME280 Register Addresses
#define BME280_REG_TEMP_MSB     0xFA
#define BME280_REG_TEMP_LSB     0xFB
#define BME280_REG_TEMP_XLSB    0xFC
#define BME280_REG_PRESS_MSB    0xF7
#define BME280_REG_PRESS_LSB    0xF8
#define BME280_REG_PRESS_XLSB   0xF9
#define BME280_REG_HUM_MSB      0xFD
#define BME280_REG_HUM_LSB      0xFE
#define BME280_REG_CONFIG       0xF5
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CTRL_HUM     0xF2
#define BME280_REG_CHIP_ID      0xD0
#define BME280_REG_RESET        0xE0

// Calibration registers
#define BME280_REG_DIG_T1       0x88
#define BME280_REG_DIG_T2       0x8A
#define BME280_REG_DIG_T3       0x8C
#define BME280_REG_DIG_P1       0x8E
#define BME280_REG_DIG_P2       0x90
#define BME280_REG_DIG_P3       0x92
#define BME280_REG_DIG_P4       0x94
#define BME280_REG_DIG_P5       0x96
#define BME280_REG_DIG_P6       0x98
#define BME280_REG_DIG_P7       0x9A
#define BME280_REG_DIG_P8       0x9C
#define BME280_REG_DIG_P9       0x9E
#define BME280_REG_DIG_H1       0xA1
#define BME280_REG_DIG_H2       0xE1
#define BME280_REG_DIG_H3       0xE3
#define BME280_REG_DIG_H4       0xE4
#define BME280_REG_DIG_H5       0xE5
#define BME280_REG_DIG_H6       0xE7

class BME280 {
public:
    BME280(uint8_t address = 0x76);
    
    bool init();
    void read();
    
    // Get sensor data
    float getTemperature() const { return temperature; }
    float getPressure() const { return pressure; }
    float getHumidity() const { return humidity; }
    float getAltitude() const { return altitude; }
    
    // Raw data getters
    int32_t getRawTemperature() const { return _rawTemperature; }
    int32_t getRawPressure() const { return _rawPressure; }
    int32_t getRawHumidity() const { return _rawHumidity; }
    
    // Configuration
    bool setSampling(uint8_t tempSampling, uint8_t pressSampling, uint8_t humSampling, uint8_t mode, uint8_t filter, uint8_t duration);
    
private:
    uint8_t address;
    float temperature, pressure, humidity, altitude;
    
    // Calibration data
    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    uint8_t dig_H1, dig_H3;
    int16_t dig_H2, dig_H4, dig_H5;
    int8_t dig_H6;
    
    // Raw data
    int32_t _rawTemperature;
    int32_t _rawPressure;
    int32_t _rawHumidity;
    
    bool loadCalibration();
    float compensateTemperature(int32_t rawTemp);
    float compensatePressure(int32_t rawPress);
    float compensateHumidity(int32_t rawHum);
    float calculateAltitude(float pressure, float seaLevelPressure = 1013.25);
};

#endif 