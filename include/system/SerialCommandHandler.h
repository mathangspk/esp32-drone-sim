#ifndef SERIALCOMMANDHANDLER_H
#define SERIALCOMMANDHANDLER_H

#include <Arduino.h>

class SerialCommandHandler {
public:
    void begin(long baudrate = 115200);
    void handle();

    // Overload cho String
    void print(const String& message);
    void println(const String& message);

    // Overload cho số
    void print(long value);
    void print(unsigned long value);
    void print(float value, int digits = 2);
    void println(long value);
    void println(unsigned long value);
    void println(float value, int digits = 2);
    void print(int value);
    void println(int value);
    void print(int16_t value);
    void println(int16_t value);   
};

#endif