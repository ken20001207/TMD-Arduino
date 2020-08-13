#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h>

#include <SoftwareSerial.h>

#define LED1_PIN 6
#define LED2_PIN 7
#define LED3_PIN 8

#define LED_NUM 10

SoftwareSerial dataSerial(2, 3);  // RX, TX

Adafruit_NeoPixel strips[] = {
    Adafruit_NeoPixel(LED_NUM, LED1_PIN, NEO_RGB + NEO_KHZ800),
    Adafruit_NeoPixel(LED_NUM, LED2_PIN, NEO_RGB + NEO_KHZ800),
    Adafruit_NeoPixel(LED_NUM, LED3_PIN, NEO_RGB + NEO_KHZ800)};

enum DisplayMode {
    SWIPE_RIGHT,
    SWIPE_LEFT,
    SWIPE_TOP,
    SWIPE_DOWN,
    BREATHING,
    NORMAL
};

DisplayMode LedMode = DisplayMode::SWIPE_DOWN;

int shouldLightNum = 5;

// Following variable should between 0 and 255
int aniVal = 0;
int red = 0;
int green = 0;
int blue = 0;

void update_leds() {
    if (aniVal < 255 / 2)
        aniVal += 1;
    else
        aniVal = 0;

    switch (LedMode) {
        uint32_t color;
        int brightness;
        case DisplayMode::NORMAL:
            color = Adafruit_NeoPixel::Color(red, green, blue);

            for (int i = 0; i < 3; i++) {
                strips[i].fill(color, 0, shouldLightNum);
                strips[i].show();
            }

            break;

        case DisplayMode::BREATHING:
            brightness = aniVal / 255;

            color = Adafruit_NeoPixel::Color(
                red * brightness, green * brightness, blue * brightness);

            for (int i = 0; i < 3; i++) {
                strips[i].fill(color);
                strips[i].show();
            }

            break;

        case DisplayMode::SWIPE_RIGHT:

            // Break ponits between 0 and 255
            int bps[] = {0, 42, 85, 127, 170, 212, 255};

            for (int i = 0; i < 3; i++) {
                brightness = 0;

                if (aniVal <= bps[i])
                    brightness = 0;
                else if (bps[i] <= aniVal && aniVal >= bps[i + 2])
                    brightness = map(aniVal, bps[i], bps[i + 2], 0, 1);
                else if (bps[i + 2] <= aniVal && aniVal >= bps[i + 4])
                    brightness = map(aniVal, 85, 170, 1, 0);
                else if (aniVal >= bps[i + 4])
                    brightness = 0;

                strips[i].fill(strips[i].Color(
                    red * brightness, green * brightness, blue * brightness));
                strips[i].show();
            }

            break;
    }
}

void setup() {
    for (int i = 0; i < 3; i++) strips[i].begin();

    MsTimer2::set(500, update_leds);
    MsTimer2::start();

    Serial.begin(9600);
    dataSerial.begin(9600);
}

String dataBuffer = "";

void loop() {
    if (dataSerial.available()) {
        char dataChar = dataSerial.read();
        dataBuffer += dataChar;

        // DEBUG POINT 1
        Serial.print(dataChar);

        if (dataBuffer.charAt(dataBuffer.length() - 1) == '\n') {
            // DEBUG POINT 2
            Serial.println(dataBuffer.c_str());

            StaticJsonDocument<1024> jsonDoc;
            DeserializationError error = deserializeJson(jsonDoc, dataBuffer);

            dataBuffer = "";

            if (!error) {
                String id = jsonDoc[0]["id"];
                Serial.println(id);
            } else {
                Serial.println(error.c_str());
                dataBuffer = "";
            }
        }
    }
}