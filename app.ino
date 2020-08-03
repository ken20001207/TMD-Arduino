#include "Adafruit_NeoPixel.h"

#define PIN 6
#define LED_NUM 10

#define ADD true
#define SUB false

int val = 0;
boolean stat = ADD;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
    strip.begin();
    strip.show();
}

void loop() {
    uint32_t color = strip.Color(val, val, val);
    for (int i = 0; i < LED_NUM; i++) strip.setPixelColor(i, color);

    // 下面是产生呼吸灯效果
    if (val >= 255) stat = SUB;
    if (val <= 0) stat = ADD;

    strip.show();
    delay(5);
    if (stat == SUB)
        val--;
    else if (stat == ADD)
        val++;
}