#include "global.h"
#include "model.h"

State::State() {
    todoDatas = new Todo[2];
    todoAmount = 0;
    displayTodoIndex = 0;
    shouldLightNum = 5;
    LedMode = DisplayMode::SWIPE_DOWN;
    aniVal = 0;
    red = 0;
    green = 0;
    blue = 0;
}

Todo State::getTodoData(int index) { return todoDatas[index]; }

Todo State::getDisplayTodo() { return todoDatas[displayTodoIndex]; }

int State::getDisplayTodoIndex() { return displayTodoIndex; }

int State::getTodoAmount() { return todoAmount; }

int State::getAniVal() { return aniVal; }

DisplayMode State::getLEDMode() { return LedMode; }

uint32_t State::getColor() {
    return Adafruit_NeoPixel::Color(red, green, blue);
}

uint32_t State::getColorByBrightness(int brightness) {
    return Adafruit_NeoPixel::Color(red * brightness, green * brightness,
                                    blue * brightness);
}

int State::getShouldLightNum() { return shouldLightNum; }

void State::setTodo(int index, char *id, char *title, char *ddl) {
    todoDatas[index].id = id;
    todoDatas[index].title = title;
    todoDatas[index].ddl = ddl;
    todoAmount = index + 1;

    if (index == getDisplayTodoIndex()) {
        printTextLCD(lcd, title, ddl);
    }
}

void State::addAniVal() {
    if (aniVal < 255 / 2)
        aniVal++;
    else
        aniVal = 0;
}

void State::setDisplayTodoIndex(int index) {
    if (todoAmount != 0) {
        if (index < todoAmount) {
            displayTodoIndex = index;
        } else {
            displayTodoIndex = todoAmount - 1;
        }

        Serial.print("Switch displaying Todo index to ");
        Serial.print(displayTodoIndex);
        Serial.print(" (");
        Serial.print(todoDatas[displayTodoIndex].title);
        Serial.println(")");

        printTextLCD(lcd, todoDatas[displayTodoIndex].title,
                     todoDatas[displayTodoIndex].ddl);
    }
}