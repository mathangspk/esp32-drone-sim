import serial
import threading
import time
from datetime import datetime

SERIAL_PORT = 'rfc2217://localhost:4000'  # Default for Wokwi/bridge
BAUD_RATE = 115200

class SerialMonitor:
    def __init__(self, port, baudrate):
        self.ser = serial.serial_for_url(port, baudrate=baudrate, timeout=0.1)
        self.running = True
        self.debug_enabled = False
        self.listen_thread = None
        self.console_mode = False
        self._start_listener()

    def _start_listener(self):
        if self.listen_thread is None or not self.listen_thread.is_alive():
            self.listen_thread = threading.Thread(target=self.listen_serial)
            self.listen_thread.daemon = True
            self.listen_thread.start()

    def _stop_listener(self):
        self.running = False
        if self.listen_thread is not None:
            self.listen_thread.join(timeout=1)
        self.running = True
        self.listen_thread = None

    def listen_serial(self):
        # Không in gì khi ở menu
        while self.running and not self.console_mode:
            time.sleep(0.1)

    def send(self, msg):
        self.ser.write((msg + '\n').encode())

    def set_debug(self, enable):
        if enable:
            self.send('DEBUG_ON')
            print("[PC] Đã gửi lệnh bật debug mode cho ESP32.")
        else:
            self.send('DEBUG_OFF')
            print("[PC] Đã gửi lệnh tắt debug mode cho ESP32.")
        self.debug_enabled = enable

    def console_view(self):
        print("\n--- CHẾ ĐỘ XEM DỮ LIỆU LIÊN TỤC (nhấn Ctrl+C để quay lại menu) ---")
        self.console_mode = True
        self._stop_listener()
        try:
            while True:
                line = self.ser.readline().decode(errors='ignore').strip()
                if line:
                    print(line)
        except KeyboardInterrupt:
            print("\n[PC] Thoát chế độ console, quay lại menu.")
        finally:
            self.console_mode = False
            self._start_listener()

    def menu(self):
        while True:
            print("\n--- MENU ---")
            print("1. Bật debug mode")
            print("2. Tắt debug mode")
            print("3. Gửi lệnh tuỳ ý")
            print("4. Xem dữ liệu liên tục (console)")
            print("5. Thoát")
            choice = input("Chọn: ").strip()
            if choice == '1':
                self.set_debug(True)
            elif choice == '2':
                self.set_debug(False)
            elif choice == '3':
                msg = input("Nhập lệnh gửi ESP32: ").strip()
                self.send(msg)
            elif choice == '4':
                self.console_view()
            elif choice == '5':
                break
            else:
                print("Lựa chọn không hợp lệ!")

    def close(self):
        self.running = False
        if self.listen_thread is not None:
            self.listen_thread.join(timeout=1)
        self.ser.close()

class SerialMonitorApp:
    def __init__(self, port=SERIAL_PORT, baudrate=BAUD_RATE):
        self.monitor = SerialMonitor(port, baudrate)

    def run(self):
        print(f"Kết nối tới {SERIAL_PORT} @ {BAUD_RATE}...")
        time.sleep(1)
        ans = input("Bạn có muốn bật debug mode không? (y/n): ").strip().lower()
        if ans == 'y':
            self.monitor.set_debug(True)
        else:
            self.monitor.set_debug(False)
        try:
            self.monitor.menu()
        except KeyboardInterrupt:
            pass
        finally:
            self.monitor.close()
            print("Đã đóng serial monitor.")

if __name__ == '__main__':
    app = SerialMonitorApp()
    app.run() 