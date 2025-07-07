# ESP32 MPU6050 I2C Project với Wokwi

Dự án này demo cách kết nối ESP32 với MPU6050 qua I2C và sử dụng serial port forwarding trong Wokwi để monitor dữ liệu sensor.

## 📋 Components

- **ESP32 Development Board** - Microcontroller
- **MPU6050** - 6-Axis Motion Sensor (Accelerometer + Gyroscope)
- **Jumper Wires** - 4 pieces cho kết nối I2C

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

## Cấu hình

### 1. File `wokwi.toml`
Đã cấu hình RFC2217 server trên port 4000:
```toml
[wokwi]
version = 1
firmware = '.pio\build\esp32doit-devkit-v1\firmware.bin'
elf = '.pio\build\esp32doit-devkit-v1\firmware.elf'
rfc2217ServerPort = 4000
```

### 2. File `platformio.ini`
Cấu hình Serial Monitor:
```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino

; Cấu hình Serial Monitor
monitor_speed = 115200
monitor_filters = esp32_exception_decoder
```

### 3. File `diagram.json`
Cấu hình Wokwi diagram với ESP32 và MPU6050:
```json
{
  "version": 1,
  "parts": [
    {"type": "board-esp32-devkit-v1", "id": "esp32"},
    {"type": "sensor-mpu6050", "id": "mpu6050"}
  ],
  "connections": [
    ["esp32:GND.1", "mpu6050:GND", "black"],
    ["esp32:3V3", "mpu6050:VCC", "red"],
    ["esp32:D21", "mpu6050:SDA", "blue"],
    ["esp32:D22", "mpu6050:SCL", "yellow"]
  ]
}
```

### 4. Code ESP32 (`src/main.cpp`)
ESP32 sẽ đọc dữ liệu từ MPU6050 và hiển thị qua Serial.

### 5. Python Scripts
- `serial_client.py` - Client gửi dữ liệu đến ESP32
- `serial_monitor.py` - Monitor hai chiều (đọc + ghi)
- `simple_monitor.py` - Monitor chỉ đọc dữ liệu từ ESP32

### 6. Debug Tools
- `i2c_scanner.ino` - I2C scanner để debug kết nối
- `MPU6050_WIRING_GUIDE.md` - Hướng dẫn chi tiết về wiring

## Cách sử dụng

### Bước 1: Cài đặt dependencies
```bash
pip install -r requirements.txt
```

### Bước 2: Upload Code
```bash
# Upload main code
pio run --target upload

# Hoặc upload I2C scanner để debug
# (Copy i2c_scanner.ino vào src/main.cpp)
pio run --target upload
```

### Bước 3: Monitor Output

#### Cách 1: PlatformIO Serial Monitor
```bash
pio device monitor
```

#### Cách 2: Python Monitor trong Cursor
```bash
# Monitor chỉ đọc (đơn giản)
python simple_monitor.py

# Monitor hai chiều (đọc + gửi tin nhắn)
python serial_monitor.py
```

## 📊 MPU6050 Sensor Data

### Dữ liệu được đọc:
- **Accelerometer**: X, Y, Z axes (16-bit raw values)
- **Gyroscope**: X, Y, Z axes (16-bit raw values)
- **Temperature**: Internal sensor (°C)

### Cấu hình hiện tại:
- **Accelerometer Range**: ±2g
- **Gyroscope Range**: ±2000°/s
- **I2C Clock**: 400kHz
- **Update Rate**: 10Hz

### Chuyển đổi dữ liệu:
```cpp
// Accelerometer (for ±2g range)
float accelX_g = accelX / 16384.0;
float accelY_g = accelY / 16384.0;
float accelZ_g = accelZ / 16384.0;

// Gyroscope (for ±2000°/s range)
float gyroX_dps = gyroX / 16.4;
float gyroY_dps = gyroY / 16.4;
float gyroZ_dps = gyroZ / 16.4;

// Temperature
float temp_celsius = temperature / 340.0 + 36.53;
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

### I2C Scanner Output
```
ESP32 I2C Scanner
==================
Scanning I2C devices...
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

1 device(s) found
Device addresses:
  0x68 - MPU6050/MPU6500
```

## 🔍 Troubleshooting

### I2C Connection Issues
1. **Kiểm tra wiring**: Đảm bảo tất cả 4 dây được kết nối đúng
2. **Chạy I2C scanner**: Upload `i2c_scanner.ino` để tìm devices
3. **Kiểm tra power**: MPU6050 cần 3.3V
4. **Kiểm tra address**: MPU6050 có thể ở 0x68 hoặc 0x69

### Common Error Messages
- **"No device found!"**: Kiểm tra wiring và power
- **"Wrong device ID"**: Kiểm tra sensor model
- **"Failed to initialize"**: Kiểm tra I2C connection

### Debug Steps
1. Upload I2C scanner code
2. Check for device detection
3. Verify correct address (0x68)
4. Test with known good sensor

## 🎯 Testing Steps

1. **Hardware Setup**
   - Connect ESP32 to MPU6050 theo wiring diagram
   - Double-check all connections
   - Power up ESP32

2. **Upload Code**
   ```bash
   pio run --target upload
   ```

3. **Monitor Output**
   ```bash
   python simple_monitor.py
   ```

4. **Verify Sensor Data**
   - Check for initialization success
   - Verify accelerometer data changes with movement
   - Verify gyroscope data changes with rotation
   - Check temperature readings

## 📚 Additional Resources

- **MPU6050 Datasheet**: [InvenSense MPU6050](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- **ESP32 I2C Documentation**: [ESP32 I2C Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html)
- **Wokwi Documentation**: [Wokwi ESP32 Guide](https://docs.wokwi.com/guides/esp32)

## 🔧 Advanced Features

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