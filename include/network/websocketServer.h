#ifndef WSSERVER_H
#define WSSERVER_H

#include <ESPAsyncWebServer.h>
#include <atomic>

class WsServer {
public:
    WsServer();
    void begin(const char* ssid = "Wokwi-GUEST", const char* password = "", int channel = 6, bool useAP = false);
    bool broadcast(const String& message);
    bool hasClient() const;

private:
    AsyncWebServer server;
    AsyncWebSocket ws;
    std::atomic<uint32_t> clientCount {0};  // Dùng kiểu atomic để thread-safe

    void connectWifi(const char* ssid, const char* password,  int channel);
    void setupAccessPoint(const char* ssid, const char* password, int channel);
};
#endif