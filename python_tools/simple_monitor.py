import serial
import time

SERIAL_PORT = 'rfc2217://localhost:4000'  # hoặc 'COMx'
BAUD_RATE = 115200

class SimpleMonitor:
    def __init__(self, port=SERIAL_PORT, baudrate=BAUD_RATE):
        self.ser = serial.serial_for_url(port, baudrate=baudrate, timeout=0.5)

    def run(self):
        print(f"Kết nối tới {SERIAL_PORT} @ {BAUD_RATE}...")
        try:
            while True:
                line = self.ser.readline().decode(errors='ignore').strip()
                if line:
                    print(line)
        except KeyboardInterrupt:
            print("\nĐã thoát simple monitor.")
        finally:
            self.ser.close()

class SimpleMonitorApp:
    def __init__(self, port=SERIAL_PORT, baudrate=BAUD_RATE):
        self.monitor = SimpleMonitor(port, baudrate)

    def run(self):
        self.monitor.run()

if __name__ == '__main__':
    app = SimpleMonitorApp()
    app.run() 