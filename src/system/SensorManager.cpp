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
    // Quét I2C để kiểm tra địa chỉ compass khi khởi động
    static bool compassChecked = false;
    if (!compassChecked) {
        uint8_t foundAddress = 0;
        for (uint8_t addr = 1; addr < 127; ++addr) {
            Wire.beginTransmission(addr);
            if (Wire.endTransmission() == 0) {
                Serial.printf("[I2C] Found device at 0x%02X\n", addr);
                if (addr == 0x0D || addr == 0x1E) {
                    foundAddress = addr;
                }
            }
        }
        if (foundAddress) {
            Serial.printf("[I2C] QMC5883L likely found at 0x%02X\n", foundAddress);
        } else {
            Serial.println("[I2C] QMC5883L NOT FOUND!");
        }
        if (!compass.begin()) {
            Serial.println("[ERROR] QMC5883L begin failed!");
        } else {
            Serial.println("[INFO] QMC5883L begin OK!");
        }
        compassChecked = true;
    }
    static unsigned long lastCompassRead = 0;
    unsigned long now = millis();
    static float mx = 0, my = 0, mz = 0;
    if (now - lastCompassRead > 30) { // tăng lên 30ms
        compass.readMag(mx, my, mz);
        lastCompassRead = now;
    }
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
    static unsigned long lastCompassRead = 0;
    unsigned long now = millis();
    static float mx = 0, my = 0, mz = 0;
    if (now - lastCompassRead > 30) { // tăng lên 30ms
        compass.readMag(mx, my, mz);
        lastCompassRead = now;
    }
#endif

    // --- Bổ sung xử lý heading gốc cho yaw ---
    static bool yawZeroed = false;
    static float yawOffset = 0.0f;
    float heading = compass.getHeading(); // đơn vị độ
    if (!yawZeroed) {
        yawOffset = heading;
        yawZeroed = true;
        attitude.setYaw(0.0f);
    }
    // Chuyển heading và yawOffset sang radian trước khi tính hiệu
    float headingRad = heading * M_PI / 180.0f;
    float yawOffsetRad = yawOffset * M_PI / 180.0f;
    float magYaw = headingRad - yawOffsetRad;
    // Chuẩn hóa magYaw về [-pi, pi]
    while (magYaw > M_PI) magYaw -= 2 * M_PI;
    while (magYaw < -M_PI) magYaw += 2 * M_PI;
    // Debug giá trị thực tế
    Serial.printf("heading: %.2f, yawOffset: %.2f, magYaw: %.2f, yaw: %.2f\n", heading, yawOffset, magYaw, attitude.getYaw());

#if USE_QMC5883L
    //Serial.printf("compass X: %.2f, Y: %.2f, Z: %.2f\n", mx, my, mz);
#endif

#if USE_MPU6050
    attitude.update(
        mpu6050.getGyroX() * M_PI / 180.0f,
        mpu6050.getGyroY() * M_PI / 180.0f,
        mpu6050.getGyroZ() * M_PI / 180.0f,
        mpu6050.getAccelX(),
        mpu6050.getAccelY(),
        mpu6050.getAccelZ(),
        magYaw
    );
#elif USE_MPU6500
    attitude.update(
        mpu6500.getGyroX() * M_PI / 180.0f,
        mpu6500.getGyroY() * M_PI / 180.0f,
        mpu6500.getGyroZ() * M_PI / 180.0f,
        mpu6500.getAccelX(),
        mpu6500.getAccelY(),
        mpu6500.getAccelZ(),
        magYaw
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