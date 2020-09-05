#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define LED_BUILTIN 2

ESP8266WiFiMulti WiFiMulti;

boolean connectioWasAlive = false;

enum EspResponse {
    CONNECTING_WIFI = 1,
    WIFI_CONNECTED = 2,
    FETCH_SUCCESS = 3,
    HTTP_NOT_OK = 4,
    REQUEST_FAILED = 5
};

void monitorWiFi() {
    if (WiFiMulti.run() != WL_CONNECTED) {
        Serial.printf("{\"code\":\"%d\", \"msg\": \"Connecting Wifi ...\"}\n",
                      EspResponse::CONNECTING_WIFI);
        blinkLED(100);
        delay(1000);
    } else {
        connectioWasAlive = true;
        Serial.printf("{\"code\":\"%d\", \"ssid\": \"%s\"}\n",
                      EspResponse::WIFI_CONNECTED, WiFi.SSID().c_str());

        blinkLED(50);
        blinkLED(50);
    }
}

void blinkLED(int duration) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
}

WiFiClient client;
HTTPClient http;

void getTodoData() {
    if (http.begin(client, "http://tmd.linyuanlin.com/api/todos/")) {
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            Serial.printf("{\"code\":\"%d\", \"data\": %s}\n",
                          EspResponse::FETCH_SUCCESS, payload.c_str());
            blinkLED(200);
            http.end();
        } else {
            Serial.printf("{\"code\":\"%d\", \"msg\": \"%s\"}\n",
                          EspResponse::HTTP_NOT_OK,
                          http.errorToString(httpCode).c_str());
            http.end();
            getTodoData();
        }

    } else {
        Serial.printf(
            "{\"code\":\"%d\", \"msg\": \"Unable to connect server\"}\n",
            EspResponse::REQUEST_FAILED);
        http.end();
        blinkLED(100);
        getTodoData();
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(4800);
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("CMCC-9518", "88888888");
}

void loop() {
    if (connectioWasAlive) {
        if (Serial.available()) {
            char t = Serial.read();
            getTodoData();
        }
    } else {
        monitorWiFi();
    }
}