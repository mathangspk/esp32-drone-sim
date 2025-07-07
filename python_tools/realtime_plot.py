import os
os.environ["PYQTGRAPH_QT_LIB"] = "PyQt5"
import sys
import re
import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtWidgets

SERIAL_PORT = 'rfc2217://localhost:4000'  # hoặc 'COMx'
BAUD_RATE = 115200

DATA_REGEX = re.compile(r"\[t=(\d+)ms\].*?Gyro\(filtered\): X=([\-\d\.]+) Y=([\-\d\.]+) Z=([\-\d\.]+) deg/s.*?Attitude: Roll=([\-\d\.]+) Pitch=([\-\d\.]+) Yaw=([\-\d\.]+) deg")

class SerialPlotter:
    def __init__(self, port, baudrate):
        self.ser = serial.serial_for_url(port, baudrate=baudrate, timeout=0.5)
        self.timestamps = []
        self.rolls = []
        self.pitches = []
        self.yaws = []
        self.gyro_x = []
        self.gyro_y = []
        self.gyro_z = []

        self.app = QtWidgets.QApplication([])
        self.win = pg.GraphicsLayoutWidget(title="ESP32 IMU Realtime Plot")
        self.win.resize(1000, 600)
        self.win.setWindowTitle('ESP32 IMU Realtime Plot')

        self.plot1 = self.win.addPlot(title="Attitude (deg)")
        self.plot1.addLegend()
        self.curve_roll = self.plot1.plot(pen='r', name='Roll')
        self.curve_pitch = self.plot1.plot(pen='g', name='Pitch')
        self.curve_yaw = self.plot1.plot(pen='b', name='Yaw')

        self.win.nextRow()
        self.plot2 = self.win.addPlot(title="Gyro (deg/s)")
        self.plot2.addLegend()
        self.curve_gx = self.plot2.plot(pen='m', name='Gyro X')
        self.curve_gy = self.plot2.plot(pen='c', name='Gyro Y')
        self.curve_gz = self.plot2.plot(pen='y', name='Gyro Z')

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(30)

    def update(self):
        while self.ser.in_waiting:
            line = self.ser.readline().decode(errors='ignore').strip()
            m = DATA_REGEX.search(line)
            if m:
                t = int(m.group(1))
                gx = float(m.group(2))
                gy = float(m.group(3))
                gz = float(m.group(4))
                roll = float(m.group(5))
                pitch = float(m.group(6))
                yaw = float(m.group(7))
                self.timestamps.append(t/1000.0)
                self.gyro_x.append(gx)
                self.gyro_y.append(gy)
                self.gyro_z.append(gz)
                self.rolls.append(roll)
                self.pitches.append(pitch)
                self.yaws.append(yaw)
                if len(self.timestamps) > 500:
                    self.timestamps = self.timestamps[-500:]
                    self.gyro_x = self.gyro_x[-500:]
                    self.gyro_y = self.gyro_y[-500:]
                    self.gyro_z = self.gyro_z[-500:]
                    self.rolls = self.rolls[-500:]
                    self.pitches = self.pitches[-500:]
                    self.yaws = self.yaws[-500:]
        self.curve_roll.setData(self.timestamps, self.rolls)
        self.curve_pitch.setData(self.timestamps, self.pitches)
        self.curve_yaw.setData(self.timestamps, self.yaws)
        self.curve_gx.setData(self.timestamps, self.gyro_x)
        self.curve_gy.setData(self.timestamps, self.gyro_y)
        self.curve_gz.setData(self.timestamps, self.gyro_z)

    def run(self):
        self.win.show()
        self.app.exec_()

class SerialPlotterApp:
    def __init__(self, port=SERIAL_PORT, baudrate=BAUD_RATE):
        self.plotter = SerialPlotter(port, baudrate)

    def run(self):
        print(f"Kết nối tới {SERIAL_PORT} @ {BAUD_RATE}...")
        self.plotter.run()

if __name__ == '__main__':
    app = SerialPlotterApp()
    app.run() 