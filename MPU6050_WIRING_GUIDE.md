# ESP32 - MPU6050 I2C Wiring Guide

## 📋 Components Required

- ESP32 Development Board
- MPU6050 6-Axis Motion Sensor
- Breadboard (optional)
- Jumper Wires (4 pieces)

## 🔌 Wiring Diagram

### Physical Connections

| ESP32 Pin | MPU6050 Pin | Wire Color | Description |
|-----------|-------------|------------|-------------|
| **3.3V**  | **VCC**     | Red        | Power Supply |
| **GND**   | **GND**     | Black      | Ground       |
| **GPIO21**| **SDA**     | Blue       | I2C Data     |
| **GPIO22**| **SCL**     | Yellow     | I2C Clock    |

### Visual Connection

```
ESP32 DevKit V1          MPU6050
┌─────────────┐         ┌─────────┐
│             │         │         │
│ 3.3V ──────┼─────────┤ VCC     │
│             │         │         │
│ GND  ──────┼─────────┤ GND     │
│             │         │         │
│ GPIO21 ────┼─────────┤ SDA     │
│             │         │         │
│ GPIO22 ────┼─────────┤ SCL     │
│             │         │         │
└─────────────┘         └─────────┘
```

## 🔧 I2C Configuration

### ESP32 I2C Pins
- **Default I2C Pins**: GPIO21 (SDA), GPIO22 (SCL)
- **I2C Clock Speed**: 400kHz (Fast Mode)
- **I2C Address**: 0x68 (MPU6050 default)

### MPU6050 I2C Address
- **Default Address**: 0x68
- **Alternative Address**: 0x69 (if AD0 pin is pulled HIGH)

## 📊 Sensor Specifications

### MPU6050 Features
- **Accelerometer**: ±2g, ±4g, ±8g, ±16g
- **Gyroscope**: ±250°/s, ±500°/s, ±1000°/s, ±2000°/s
- **Temperature Sensor**: -40°C to +85°C
- **I2C Interface**: Up to 400kHz
- **Operating Voltage**: 3.3V

### Current Configuration
- **Accelerometer Range**: ±2g
- **Gyroscope Range**: ±2000°/s
- **Low Pass Filter**: 44Hz bandwidth

## 🚀 Code Features

### Initialization
1. **I2C Setup**: 400kHz clock speed
2. **Device Detection**: WHO_AM_I register check
3. **Power Management**: Wake up from sleep mode
4. **Sensor Configuration**: Set ranges and filters

### Data Reading
- **Accelerometer**: X, Y, Z axes (16-bit)
- **Gyroscope**: X, Y, Z axes (16-bit)
- **Temperature**: Internal sensor
- **Update Rate**: 10Hz (100ms delay)

## 🔍 Troubleshooting

### Common Issues

#### 1. "No device found!" Error
**Possible Causes:**
- Wrong I2C address
- Loose connections
- Power supply issues
- Damaged sensor

**Solutions:**
- Check all wire connections
- Verify power supply (3.3V)
- Try I2C scanner to find address
- Test with different sensor

#### 2. "Wrong device ID" Error
**Possible Causes:**
- Different sensor model
- I2C address conflict
- Hardware damage

**Solutions:**
- Check sensor model
- Verify I2C address
- Test with known good sensor

#### 3. Unstable Readings
**Possible Causes:**
- Loose connections
- Power supply noise
- Sensor placement issues

**Solutions:**
- Secure all connections
- Use stable power supply
- Mount sensor properly
- Add capacitors for filtering

### I2C Scanner Code
```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("I2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices = 0;
  
  Serial.println("Scanning...");
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  }
  
  delay(5000);
}
```

## 📈 Expected Output

### Serial Monitor Output
```
ESP32 MPU6050 I2C Test
MPU6050 found!
MPU6050 initialized successfully!
ESP32 đang chờ dữ liệu từ Python...
Accel: X=123 Y=-45 Z=16543 | Gyro: X=12 Y=-3 Z=8 | Temp: 25.3°C
Accel: X=125 Y=-43 Z=16541 | Gyro: X=11 Y=-2 Z=9 | Temp: 25.4°C
```

### Data Interpretation
- **Accelerometer**: Raw values (divide by 16384 for ±2g range)
- **Gyroscope**: Raw values (divide by 16.4 for ±2000°/s range)
- **Temperature**: Convert using formula: `temp = (raw_value / 340.0) + 36.53`

## 🎯 Testing Steps

1. **Hardware Setup**
   - Connect all wires according to diagram
   - Double-check connections
   - Power up ESP32

2. **Upload Code**
   ```bash
   pio run --target upload
   ```

3. **Monitor Output**
   ```bash
   python simple_monitor.py
   ```

4. **Verify Readings**
   - Check for initialization success
   - Verify sensor data is changing
   - Test sensor movement response

## 🔧 Advanced Configuration

### Custom I2C Pins
```cpp
// Use different I2C pins
Wire.begin(SDA_PIN, SCL_PIN);
```

### Different Sensor Ranges
```cpp
// Accelerometer: ±4g
Wire.write(0x08);

// Gyroscope: ±500°/s
Wire.write(0x08);
```

### Interrupt Configuration
```cpp
// Enable data ready interrupt
Wire.write(0x38); // INT_ENABLE register
Wire.write(0x01); // Enable DATA_RDY interrupt
``` 