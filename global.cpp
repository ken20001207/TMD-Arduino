#include "global.h"
#include "model.h"

#define LED1_PIN 6
#define LED2_PIN 7
#define LED3_PIN 8

#define LED_NUM 50

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Adafruit_NeoPixel strips[] = {
    Adafruit_NeoPixel(LED_NUM, LED1_PIN, NEO_RGB + NEO_KHZ800),
    Adafruit_NeoPixel(LED_NUM, LED2_PIN, NEO_RGB + NEO_KHZ800),
    Adafruit_NeoPixel(LED_NUM, LED3_PIN, NEO_RGB + NEO_KHZ800)};

void printTextLCD(LiquidCrystal_I2C lcd, char *line1, char *line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}