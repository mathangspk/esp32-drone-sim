#include "network/httpServer.h"
#include <WiFi.h>
#include <uri/UriBraces.h>

HttpServer::HttpServer()
    : server(80), led1Pin(26), led2Pin(27), led1State(false), led2State(false) {}

void HttpServer::begin(const char* ssid, const char* password, int channel) {
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);

    // Sử dụng Access Point thay vì Station
    WiFi.softAP(ssid, password, channel);
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", [this]() { sendHtml(); });
    server.on(UriBraces("/toggle/{}"), [this]() { handleToggle(); });

    server.begin();
    Serial.println("HTTP server started (AP mode)");
}

void HttpServer::handle() {
    server.handleClient();
}

void HttpServer::sendHtml() {
    String response = R"(
    <!DOCTYPE html><html>
      <head>
        <title>ESP32 Web Server Demo</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
          html { font-family: sans-serif; text-align: center; }
          body { display: inline-flex; flex-direction: column; }
          h1 { margin-bottom: 1.2em; } 
          h2 { margin: 0; }
          div { display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: auto auto; grid-auto-flow: column; grid-gap: 1em; }
          .btn { background-color: #5B5; border: none; color: #fff; padding: 0.5em 1em;
                 font-size: 2em; text-decoration: none }
          .btn.OFF { background-color: #333; }
        </style>
      </head>
      <body>
        <h1>ESP32 Web Server</h1>
        <div>
          <h2>LED 1</h2>
          <a href="/toggle/1" class="btn LED1_TEXT">LED1_TEXT</a>
          <h2>LED 2</h2>
          <a href="/toggle/2" class="btn LED2_TEXT">LED2_TEXT</a>
        </div>
      </body>
    </html>
    )";
    response.replace("LED1_TEXT", led1State ? "ON" : "OFF");
    response.replace("LED2_TEXT", led2State ? "ON" : "OFF");
    server.send(200, "text/html", response);
}

void HttpServer::handleToggle() {
    String led = server.pathArg(0);
    Serial.print("Toggle LED #");
    Serial.println(led);

    switch (led.toInt()) {
        case 1:
            led1State = !led1State;
            digitalWrite(led1Pin, led1State);
            break;
        case 2:
            led2State = !led2State;
            digitalWrite(led2Pin, led2State);
            break;
    }
    sendHtml();
}