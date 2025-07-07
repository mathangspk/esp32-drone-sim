#include <Arduino.h>
#include "system/SerialCommandHandler.h"
#include "system/SensorManager.h"
#include "system/SensorLogger.h"
#include "config.h"
#include "core/IMUData.h"
#include "network/httpServer.h"
#include "network/websocketServer.h"
#include "receiver/IBusReceiver.h"

volatile bool debugMode = false;

// Communication and sensor handlers
SerialCommandHandler serial;
SensorManager sensors(serial);
SensorLogger logger(serial);
IBusReceiver receiver(&Serial2);
//HttpServer httpServer;
WsServer wsServer;

QueueHandle_t sensorDataQueue;

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

void sensorTask(void *pvParameters) {
    SensorManager* sensorManager = (SensorManager*)pvParameters;
    SensorData data;
    while (1) {
        
        receiver.read_channel_data();
        sensorManager->readAll();
        data.mpu6050 = sensorManager->getMPU6050();
        data.mpu6500 = sensorManager->getMPU6500();
        data.bme280 = sensorManager->getBME280();
        data.compass = sensorManager->getQMC5883L();
        const auto& att = sensorManager->getAttitude();
        data.roll_deg = att.getRoll() * 180.0f / M_PI;
        data.pitch_deg = att.getPitch() * 180.0f / M_PI;
        data.yaw_deg = att.getYaw() * 180.0f / M_PI;
        data.gyroX_dps = data.mpu6050.getGyroX();
        data.gyroY_dps = data.mpu6050.getGyroY();
        data.gyroZ_dps = data.mpu6050.getGyroZ();
        data.throttle = receiver.get_throttle();
        data.roll     = receiver.get_roll();
        data.pitch    = receiver.get_pitch();
        data.yaw      = receiver.get_yaw();
        data.aux1     = receiver.get_aux1();
        data.aux2     = receiver.get_aux2();
        data.timestamp_ms = millis();
        if(wsServer.hasClient()) {
            //Serial.printf("[Sensor Task] Data ready at %lu ms\n", data.timestamp_ms);
            if (xQueueSend(sensorDataQueue, &data, 0) != pdTRUE) {
                Serial.println("[WARN] Queue full! Data dropped.");
                xQueueReset(sensorDataQueue); // flush queue để hệ thống không treo
            }
        }
        //xQueueOverwrite(sensorDataQueue, &data);
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
}

void displayTask(void *pvParameters) {
    SensorData buffer[10];
    int index = 0;

    while (1) {
       
        while (uxQueueMessagesWaiting(sensorDataQueue) > 0) {
            SensorData data;
            if (xQueueReceive(sensorDataQueue, &data, 0) == pdTRUE) {
                buffer[index++] = data;

                if (index >= 10) {
                    char json[1024];
                    int len = snprintf(json, sizeof(json), "{\"type\":\"batch\",\"data\":[");
                    for (int i = 0; i < 10; ++i) {
                        len += snprintf(json + len, sizeof(json) - len,
                            "[%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%u,%u,%u,%u,%u,%u]%s",
                            buffer[i].timestamp_ms,
                            buffer[i].roll_deg,
                            buffer[i].pitch_deg,
                            buffer[i].yaw_deg,
                            buffer[i].gyroX_dps,
                            buffer[i].gyroY_dps,
                            buffer[i].gyroZ_dps,
                            buffer[i].throttle,
                            buffer[i].roll,
                            buffer[i].pitch,
                            buffer[i].yaw,
                            buffer[i].aux1,
                            buffer[i].aux2,
                            (i < 9) ? "," : ""
                        );
                    }
                    snprintf(json + len, sizeof(json) - len, "]}");
                    //Serial.print(json);
                    if (!wsServer.broadcast(String(json))) {
                        Serial.printf("[WARN] Batch broadcast failed!\n");
                    }

                    index = 0;
                    vTaskDelay(10 / portTICK_PERIOD_MS); // Tránh nghẽn mạng
                }
            }
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);  // Giải phóng CPU
    }
}

void memoryMonitorTask(void *pvParameters) {
    while (1) {
        Serial.printf("[RAM] Free heap: %u bytes\n", ESP.getFreeHeap());
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // 2 giây
    }
}



void setup() {
    // Initialize serial communication
    Serial.printf("Reset reason: %d\n", esp_reset_reason());
    serial.begin(115200);
    DEBUG_PRINTLN("ESP32 Multi-Sensor System (I2C + SPI)");
    DEBUG_PRINTLN("=====================================");
    
    // Initialize all sensors
    if (sensors.initAll()) {
        DEBUG_PRINTLN("All sensors initialized successfully!");
    } else {
        DEBUG_PRINTLN("Some sensors failed to initialize!");
    }
    
    DEBUG_PRINTLN("Starting sensor monitoring...");
    DEBUG_PRINTLN("=============================");

    receiver.begin();


    Serial.printf("[Sensors] Free heap: %u\n", ESP.getFreeHeap());

    //sensorDataQueue = xQueueCreate(1, sizeof(SensorData)); // chỉ giữ bản ghi mới nhất
    sensorDataQueue = xQueueCreate(600, sizeof(SensorData)); // giữ 100 mẫu

    Serial.printf("[after-queue] Free heap: %u\n", ESP.getFreeHeap());

    xTaskCreatePinnedToCore(sensorTask, "Sensor Task", 4096, &sensors, 1, NULL, 0);
    xTaskCreatePinnedToCore(displayTask, "Display Task", 8192, &logger, 1, NULL, 1);
    xTaskCreatePinnedToCore(memoryMonitorTask, "Memory Monitor", 2048, NULL, 1, NULL, 1);


    Serial.printf("[after-task-create] Free heap: %u\n", ESP.getFreeHeap());
    //httpServer.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
    wsServer.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, false);
}

void loop() {
    // Không cần code ở đây
    serial.handle();
    //httpServer.handle();
    //wsServer.broadcast("Hello from ESP32!"); // Gửi tin nhắn đến tất cả client kết nối
}