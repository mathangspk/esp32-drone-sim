# ESP32 Sensor Dashboard

Dashboard web để hiển thị và ghi log dữ liệu cảm biến từ ESP32 qua WebSocket.

## Cấu trúc file

```
html/
├── index.html          # Trang chính
├── css/
│   └── style.css       # Styles CSS
├── js/
│   ├── utils.js        # Hàm tiện ích và xử lý dữ liệu
│   ├── stats.js        # Tính toán thống kê timestamp gap
│   ├── logging.js      # Chức năng ghi log CSV
│   └── websocket.js    # Quản lý kết nối WebSocket
└── README.md           # Hướng dẫn này
```

## Cách sử dụng

### 1. Chạy server cục bộ
```bash
cd html
python3 -m http.server 8080
```

### 2. Mở trình duyệt
Truy cập: `http://localhost:8080/index.html`

### 3. Kết nối WebSocket
- Click "🔌 Connect" để kết nối với ESP32
- WebSocket server chạy trên port 8180

### 4. Ghi log dữ liệu
- Click "⏺️ Start Logging" để bắt đầu ghi
- Click "⏹️ Stop Logging" để dừng ghi
- Click "📥 Download CSV" để tải file log

## Tính năng

- **Real-time display**: Hiển thị dữ liệu cảm biến theo thời gian thực
- **CSV logging**: Ghi log dữ liệu vào file CSV
- **Statistics**: Thống kê khoảng cách timestamp
- **Gap detection**: Phát hiện khoảng trống trong dữ liệu
- **Clear log**: Xóa log hiển thị
- **Reset stats**: Reset thống kê

## Dữ liệu được ghi

File CSV chứa các cột:
- Time (ms): Thời gian
- Roll, Pitch, Yaw: Góc nghiêng
- GyroX, GyroY, GyroZ: Tốc độ góc
- Throttle: Gas
- RC_Roll, RC_Pitch, RC_Yaw: Tín hiệu điều khiển
- Aux1, Aux2: Kênh phụ

## Troubleshooting

### Không lưu được CSV trên Linux/Firefox
1. Đảm bảo chạy qua server (không mở file trực tiếp)
2. Kiểm tra quyền ghi thư mục Download
3. Kiểm tra Console (F12) có lỗi không

### WebSocket không kết nối
1. Đảm bảo ESP32 đang chạy WebSocket server
2. Kiểm tra port 8180 có đúng không
3. Kiểm tra firewall/antivirus 