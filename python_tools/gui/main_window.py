from PyQt5 import QtWidgets
from .plot_widget import PlotWidget
from ..core.serial_manager import SerialManager
from ..core.data_parser import DataParser

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('ESP32 IMU GUI')
        self.resize(1200, 700)

        # Central widget
        central = QtWidgets.QWidget()
        self.setCentralWidget(central)
        layout = QtWidgets.QVBoxLayout(central)

        # Toolbar: chọn cổng, baudrate, nút kết nối, debug
        toolbar = QtWidgets.QHBoxLayout()
        self.port_combo = QtWidgets.QComboBox()
        self.baud_combo = QtWidgets.QComboBox()
        self.baud_combo.addItems(['115200', '57600', '38400', '9600'])
        self.connect_btn = QtWidgets.QPushButton('Kết nối')
        self.disconnect_btn = QtWidgets.QPushButton('Ngắt')
        self.debug_btn = QtWidgets.QPushButton('Bật debug mode')
        toolbar.addWidget(QtWidgets.QLabel('Cổng:'))
        toolbar.addWidget(self.port_combo)
        toolbar.addWidget(QtWidgets.QLabel('Baudrate:'))
        toolbar.addWidget(self.baud_combo)
        toolbar.addWidget(self.connect_btn)
        toolbar.addWidget(self.disconnect_btn)
        toolbar.addWidget(self.debug_btn)
        toolbar.addStretch()
        layout.addLayout(toolbar)

        # Plot
        self.plot_widget = PlotWidget()
        layout.addWidget(self.plot_widget)

        # Log trạng thái
        self.log_box = QtWidgets.QTextEdit()
        self.log_box.setReadOnly(True)
        self.log_box.setMaximumHeight(120)
        layout.addWidget(self.log_box)

        # Serial manager, data parser
        self.serial = SerialManager()
        self.parser = DataParser()

        # TODO: Kết nối signal/slot cho các nút, cập nhật log, plot, v.v. 