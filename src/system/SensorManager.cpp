#include "system/SensorManager.h"
#include "config.h"
#include <math.h>
#include "sensors/I2CDeviceManager.h"
#include "sensors/SPIDeviceManager.h"

// Example: Biquad filters for gyro data
static BiquadLPF gyroXFilter, gyroYFilter, gyroZFilter;
static bool filtersInitialized = false;

SensorManager::SensorManager(SerialCommandHandler& serialHandler)
    : serial(serialHandler)
#if USE_MPU6500 && MPU6500_USE_SPI
    , mpu6500(5)
#endif
{
    float cutoff = GYRO_FILTER_CUTOFF, sampleRate = GYRO_SAMPLE_RATE;
    for (int i = 0; i < 3; ++i) {
#if USE_MPU6050
        gyro6050Filter[i].setup(cutoff, sampleRate);
        accel6050Filter[i].setup(cutoff, sampleRate);
#endif
#if USE_MPU6500
        gyro6500Filter[i].setup(cutoff, sampleRate);
        accel6500Filter[i].setup(cutoff, sampleRate);
#endif
    }
    attitude.setup(0.98f, 1.0f / sampleRate);
}

bool SensorManager::initAll() {
#if USE_MPU6050 || USE_BME280 || USE_QMC5883L
    I2CDeviceManager::begin();
    I2CDeviceManager::scan();
#endif
#if USE_MPU6500 && MPU6500_USE_SPI
    SPIDeviceManager::begin();
#endif
    bool allSuccess = true;
#if USE_MPU6050
    allSuccess &= initSensor("MPU6050 (I2C)", mpu6050.init());
#endif
#if USE_MPU6500
    allSuccess &= initSensor("MPU6500 (SPI)", mpu6500.init());
#endif
#if USE_BME280
    allSuccess &= initSensor("BME280 (I2C)", bme.init());
#endif
#if USE_QMC5883L
    allSuccess &= initSensor("QMC5883L (I2C)", compass.init());
#endif
    
    // Setup filters (Betaflight style)
    if (!filtersInitialized) {
        float cutoff = 90.0f; // Hz
        float sampleRate = 1000.0f; // Hz
        gyroXFilter.setup(cutoff, sampleRate);
        gyroYFilter.setup(cutoff, sampleRate);
        gyroZFilter.setup(cutoff, sampleRate);
        filtersInitialized = true;
    }
    
    return allSuccess;
}

void SensorManager::readAll() {
#if USE_MPU6050
    mpu6050.read();
#endif
#if USE_MPU6500
    mpu6500.read();
#endif
#if USE_BME280
    bme.read();
#endif
#if USE_QMC5883L
    compass.read();
#endif

#if USE_MPU6050
    // Lấy dữ liệu trực tiếp từ mpu6050
    attitude.update(
        mpu6050.getGyroX() * M_PI / 180.0f,
        mpu6050.getGyroY() * M_PI / 180.0f,
        mpu6050.getGyroZ() * M_PI / 180.0f,
        mpu6050.getAccelX(),
        mpu6050.getAccelY(),
        mpu6050.getAccelZ()
    );
#elif USE_MPU6500
    // Lấy dữ liệu trực tiếp từ mpu6500
    attitude.update(
        mpu6500.getGyroX() * M_PI / 180.0f,
        mpu6500.getGyroY() * M_PI / 180.0f,
        mpu6500.getGyroZ() * M_PI / 180.0f,
        mpu6500.getAccelX(),
        mpu6500.getAccelY(),
        mpu6500.getAccelZ()
    );
#endif
}

bool SensorManager::initSensor(const char* name, bool success) {
    if (success) {
        serial.print(name); serial.println(": OK");
    } else {
        serial.print(name); serial.println(": FAILED");
    }
    return success;
}