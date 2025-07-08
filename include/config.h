#ifndef CONFIG_H
#define CONFIG_H

// Enable/disable sensors
#define USE_MPU6050   0
#define USE_MPU6500   1
#define USE_BME280    0
#define USE_QMC5883L  1

// Select interface for sensors that support multiple
#define MPU6500_USE_SPI   1
#define MPU6500_USE_I2C   0

// Filter and sample rate settings
#define GYRO_FILTER_CUTOFF   90.0f
#define GYRO_SAMPLE_RATE     1000.0f

// Debug mode: runtime variable
extern volatile bool debugMode;
#define DEBUG_PRINT(x)    do { if (debugMode) Serial.print(x); } while(0)
#define DEBUG_PRINTLN(x)  do { if (debugMode) Serial.println(x); } while(0)
inline void debugPrintFloat(float value, int digits) { if (debugMode) Serial.print(value, digits); }

#endif 