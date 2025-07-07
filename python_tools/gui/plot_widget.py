from PyQt5 import QtWidgets
import pyqtgraph as pg

class PlotWidget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.layout = QtWidgets.QVBoxLayout(self)
        self.plot_widget = pg.GraphicsLayoutWidget()
        self.layout.addWidget(self.plot_widget)
        # TODO: Thêm các plot attitude, gyro ở đây 