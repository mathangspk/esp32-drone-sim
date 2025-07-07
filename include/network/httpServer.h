#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <WebServer.h>

class HttpServer {
public:
    HttpServer();
    void begin(const char* ssid, const char* password, int channel = 6);
    void handle();

private:
    WebServer server;
    int led1Pin, led2Pin;
    bool led1State, led2State;

    void sendHtml();
    void handleToggle();
};

#endif