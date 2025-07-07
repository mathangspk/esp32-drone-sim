#include "system/SensorLogger.h"
#include "config.h"

extern volatile bool debugMode;

SensorLogger::SensorLogger(SerialCommandHandler& serialHandler) : serial(serialHandler) {}

void SensorLogger::logAllSensors(
    #if USE_MPU6050
    const MPU6050& mpu6050,
    #endif
    #if USE_MPU6500
    const MPU6500& mpu6500,
    #endif
    #if USE_BME280
    const BME280& bme,
    #endif
    #if USE_QMC5883L
    const QMC5883L& compass,
    #endif
    float roll_deg, float pitch_deg, float yaw_deg,
    float gyroX_dps, float gyroY_dps, float gyroZ_dps,
    unsigned long timestamp_ms
) {
    static unsigned long frame_id = 0;
    static unsigned long lastTimestamp = 0;
    unsigned long delta = timestamp_ms - lastTimestamp;
    lastTimestamp = timestamp_ms;

    float freq = 0;
    if (delta > 0) freq = 1000.0f / delta; // Hz

    if (debugMode) {
        serial.print("{");
        serial.print("\"frame_id\":"); serial.print(frame_id++); // Thêm dòng này
        serial.print(",\"timestamp\":"); serial.print(timestamp_ms);
        serial.print(",\"cycle_ms\":"); serial.print(delta);
        serial.print(",\"freq_hz\":"); serial.print(freq, 2);

        #if USE_MPU6050
        serial.print(",\"mpu6050\":{");
        serial.print("\"accelX\":"); serial.print(mpu6050.getAccelX()); serial.print(",");
        serial.print("\"accelY\":"); serial.print(mpu6050.getAccelY()); serial.print(",");
        serial.print("\"accelZ\":"); serial.print(mpu6050.getAccelZ()); serial.print(",");
        serial.print("\"gyroX\":"); serial.print(mpu6050.getGyroX()); serial.print(",");
        serial.print("\"gyroY\":"); serial.print(mpu6050.getGyroY()); serial.print(",");
        serial.print("\"gyroZ\":"); serial.print(mpu6050.getGyroZ());
        serial.print("}");
        #endif

        #if USE_MPU6500
        serial.print(",\"mpu6500\":{");
        serial.print("\"accelX\":"); serial.print(mpu6500.getAccelX()); serial.print(",");
        serial.print("\"accelY\":"); serial.print(mpu6500.getAccelY()); serial.print(",");
        serial.print("\"accelZ\":"); serial.print(mpu6500.getAccelZ()); serial.print(",");
        serial.print("\"gyroX\":"); serial.print(mpu6500.getGyroX()); serial.print(",");
        serial.print("\"gyroY\":"); serial.print(mpu6500.getGyroY()); serial.print(",");
        serial.print("\"gyroZ\":"); serial.print(mpu6500.getGyroZ());
        serial.print("}");
        #endif

        #if USE_BME280
        serial.print(",\"bme280\":{");
        serial.print("\"temp\":"); serial.print(bme.readTemperature()); serial.print(",");
        serial.print("\"press\":"); serial.print(bme.readPressure()); serial.print(",");
        serial.print("\"hum\":"); serial.print(bme.readHumidity());
        serial.print("}");
        #endif

        #if USE_QMC5883L
        serial.print(",\"compass\":{");
        serial.print("\"x\":"); serial.print(compass.getX()); serial.print(",");
        serial.print("\"y\":"); serial.print(compass.getY()); serial.print(",");
        serial.print("\"z\":"); serial.print(compass.getZ());
        serial.print("}");
        #endif

        serial.print(",\"attitude\":{");
        serial.print("\"roll\":"); serial.print(roll_deg); serial.print(",");
        serial.print("\"pitch\":"); serial.print(pitch_deg); serial.print(",");
        serial.print("\"yaw\":"); serial.print(yaw_deg);
        serial.print("}");

        serial.println("}");
    }
}