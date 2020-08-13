#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

boolean connectioWasAlive = false;

void monitorWiFi() {
    if (WiFiMulti.run() != WL_CONNECTED) {
        Serial.printf("{\"msg\": \"Connecting Wifi ...\"}\n");
        blinkLED(100);

        delay(1000);
    } else {
        connectioWasAlive = true;
        Serial.printf("{\"msg\": \"connected to %s \"}\n", WiFi.SSID().c_str());

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

void getTodoData() {
    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, "http://tmd.linyuanlin.com/api/todos/")) {
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            Serial.println(payload);
            blinkLED(200);
            delay(2000);

        } else {
            Serial.printf("{\"msg\": \"%s\"}\n",
                          http.errorToString(httpCode).c_str());
        }

        http.end();

    } else {
        Serial.printf("{\"msg\": \"Unable to connect server\"}\n");
        blinkLED(100);
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(9600);

    WiFi.mode(WIFI_STA);

    WiFiMulti.addAP("Yuanlin’s iPhone 11 Pro", "5408854088");
    WiFiMulti.addAP("Gleetour Hotel SH");
}

void loop() {
    if (connectioWasAlive)
        getTodoData();
    else
        monitorWiFi();
}