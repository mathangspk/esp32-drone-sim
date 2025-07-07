#ifndef QMC5883L_H
#define QMC5883L_H

#include "I2CDeviceManager.h"

class QMC5883L {
public:
    QMC5883L(uint8_t address = 0x0D);
    
    bool init();
    void read();
    
    // Get sensor data
    int16_t getX() const { return x; }
    int16_t getY() const { return y; }
    int16_t getZ() const { return z; }
    int16_t getTemperature() const { return temperature; }
    
    // Calculate heading in degrees
    float getHeading() const;
    
private:
    uint8_t address;
    int16_t x, y, z;
    int16_t temperature;
};

#endif 