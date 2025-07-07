from gui.main_window import MainWindow
from PyQt5 import QtWidgets
import sys

def main():
    app = QtWidgets.QApplication([])
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main() 