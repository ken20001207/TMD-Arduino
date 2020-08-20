#ifndef __global_h__
#define __global_h__
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <paj7620.h>
#include <Adafruit_NeoPixel.h>
extern LiquidCrystal_I2C lcd;
void printTextLCD(LiquidCrystal_I2C lcd, char *line1, char *line2);
#endif