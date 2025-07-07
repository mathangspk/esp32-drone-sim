#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("ESP32 I2C Scanner");
  Serial.println("==================");
}

void loop() {
  byte error, address;
  int nDevices = 0;
  
  Serial.println("Scanning I2C devices...");
  Serial.println("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
  
  for(address = 1; address < 127; address++) {
    // Print address range header
    if (address % 16 == 0) {
      Serial.println();
      Serial.print(address / 16, HEX);
      Serial.print("0 ");
    }
    
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print(" ");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print(" XX");
    } else {
      Serial.print(" --");
    }
  }
  
  Serial.println();
  Serial.println();
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.print(nDevices);
    Serial.println(" device(s) found");
    
    // List found devices
    Serial.println("Device addresses:");
    for(address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      
      if (error == 0) {
        Serial.print("  0x");
        if (address < 16) Serial.print("0");
        Serial.print(address, HEX);
        
        // Identify common devices
        switch(address) {
          case 0x68:
            Serial.println(" - MPU6050/MPU6500");
            break;
          case 0x69:
            Serial.println(" - MPU6050/MPU6500 (AD0=HIGH)");
            break;
          case 0x3C:
            Serial.println(" - OLED Display");
            break;
          case 0x27:
            Serial.println(" - LCD Display (PCF8574)");
            break;
          case 0x48:
            Serial.println(" - ADS1115 ADC");
            break;
          default:
            Serial.println(" - Unknown device");
            break;
        }
      }
    }
  }
  
  Serial.println();
  Serial.println("Scan completed. Waiting 5 seconds before next scan...");
  Serial.println("==================================================");
  delay(5000);
} 