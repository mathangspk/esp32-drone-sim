#include "network/websocketServer.h"

WsServer::WsServer() : server(80), ws("/ws") {}

void WsServer::connectWifi(const char* ssid, const char* password,  int channel) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password, channel);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi.");
    Serial.print("[Wifi] IP Address: ");
    Serial.println(WiFi.localIP());
}

void WsServer::setupAccessPoint(const char* ssid, const char* password, int channel) {
    WiFi.mode(WIFI_AP);
    bool result;

    if(password && strlen(password) > 0) {
        result = WiFi.softAP(ssid, password, channel);
    } else {
        result = WiFi.softAP(ssid, nullptr, channel);
    }
    if(result){
        Serial.println("Access Point started successfully.");
        Serial.print("[Wifi] IP Address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("Failed to start Access Point.");
    }
    WiFi.softAP(ssid, password, channel);
    Serial.print("WebSocket server started at: ");
    Serial.println(WiFi.softAPIP());
}

void WsServer::begin(const char* ssid, const char* password, int channel, bool useAP) {
    if(useAP) {
        setupAccessPoint(ssid, password, channel);
    } else {
        connectWifi(ssid, password, channel);
    }
    //setupAccessPoint(ssid, password, channel);
    clientCount = 0;
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
        //Serial.printf("[WebSocket] Event: %s for Client #%u\n", type, client->id());
        switch (type) {
            case WS_EVT_CONNECT:
                Serial.printf("[WebSocket] Client #%u connected. Total: %u\n", client->id(), ws.count());
                break;
            case WS_EVT_DISCONNECT:
                if (clientCount > 0) clientCount--;
                Serial.printf("[WebSocket] Client #%u disconnected. Total: %u\n", client->id(), ws.count());
                break;
            default:
                break;
        }
    });

    server.addHandler(&ws);

    //ws.enableHeartbeat(10000, 3000, 2);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "WebSocket server running!");
    });

    server.begin();
    Serial.println("WebSocket server started.");
}

bool WsServer::broadcast(const String& message) {
    if (ws.count() == 0) return false;
    ws.textAll(message);
    return true;
}

bool WsServer::hasClient() const {
    return ws.count() > 0;
}