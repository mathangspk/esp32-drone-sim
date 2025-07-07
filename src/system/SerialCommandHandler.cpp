#include "system/SerialCommandHandler.h"
#include "config.h"

void SerialCommandHandler::begin(long baudrate) {
    Serial.begin(baudrate);
}

void SerialCommandHandler::handle() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("DEBUG_ON")) debugMode = true;
        else if (cmd.equalsIgnoreCase("DEBUG_OFF")) debugMode = false;
        else {
            DEBUG_PRINT("Python: "); DEBUG_PRINTLN(cmd);
            DEBUG_PRINT("ESP32: Received '"); DEBUG_PRINT(cmd); DEBUG_PRINTLN("' successfully!");
        }
    }
}

void SerialCommandHandler::print(const String& message) {
    Serial.print(message);
}
void SerialCommandHandler::println(const String& message) {
    Serial.println(message);
}
void SerialCommandHandler::print(long value) {
    Serial.print(value);
}
void SerialCommandHandler::print(unsigned long value) {
    Serial.print(value);
}
void SerialCommandHandler::print(float value, int digits) {
    Serial.print(value, digits);
}
void SerialCommandHandler::println(long value) {
    Serial.println(value);
}
void SerialCommandHandler::println(unsigned long value) {
    Serial.println(value);
}
void SerialCommandHandler::println(float value, int digits) {
    Serial.println(value, digits);
}
void SerialCommandHandler::print(int value) {
    Serial.print(value);
}
void SerialCommandHandler::println(int value) {
    Serial.println(value);
}
void SerialCommandHandler::print(int16_t value) {
    Serial.print(value);
}
void SerialCommandHandler::println(int16_t value) {
    Serial.println(value);
}