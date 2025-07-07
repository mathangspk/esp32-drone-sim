import serial

class SerialManager:
    def __init__(self, port=None, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        # TODO: Thêm các thuộc tính quản lý trạng thái, callback, v.v.

    def connect(self):
        # TODO: Kết nối serial
        pass

    def disconnect(self):
        # TODO: Ngắt kết nối serial
        pass

    def send(self, msg):
        # TODO: Gửi lệnh qua serial
        pass

    def read_loop(self):
        # TODO: Đọc dữ liệu liên tục, emit signal/callback khi có dòng mới
        pass 