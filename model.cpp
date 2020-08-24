#include "global.h"
#include "model.h"

#define MAX_BRIGHTNESS 0.1

State::State() {
    todoDatas = new Todo[10];
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

void State::setTodo(int index, char *title, char *ddl, char *status) {
    todoDatas[index].title = title;
    todoDatas[index].ddl = ddl;
    todoDatas[index].status = status;
    todoAmount = index + 1;

    if (index == getDisplayTodoIndex()) {
        printTextLCD(lcd, title, ddl);
    }

    setLEDMode(DisplayMode::NORMAL);

    if (strcmp(todoDatas[displayTodoIndex].status, "ONE_DAY_LEFT") == 0) {
        setShouldLightNum(50);
        setDisplayColor(255, 0, 0);
    } else if (strcmp(todoDatas[displayTodoIndex].status, "ONE_WEEK_LEFT") ==
               0) {
        setShouldLightNum(35);
        setDisplayColor(255, 170, 0);
    } else {
        setShouldLightNum(15);
        setDisplayColor(0, 255, 0);
    }
}

void State::addAniVal() {
    if (aniVal < 255)
        aniVal++;
    else
        aniVal = 0;

    updateLEDs();
}

void State::setDisplayColor(int r, int g, int b) {
    red = r;
    green = g;
    blue = b;
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

        setLEDMode(DisplayMode::NORMAL);

        if (strcmp(todoDatas[displayTodoIndex].status, "ONE_DAY_LEFT") == 0) {
            setShouldLightNum(50);
            setDisplayColor(255, 0, 0);
        } else if (strcmp(todoDatas[displayTodoIndex].status,
                          "ONE_WEEK_LEFT") == 0) {
            setShouldLightNum(25);
            setDisplayColor(255, 170, 0);
        } else {
            setShouldLightNum(10);
            setDisplayColor(0, 255, 0);
        }
    }
}

void State::setLEDMode(DisplayMode mode) { LedMode = mode; }

void State::setShouldLightNum(int num) { shouldLightNum = num; }

uint32_t color;
double brightness;
void State::updateLEDs() {
    switch (getLEDMode()) {
        case DisplayMode::NORMAL:

            for (int i = 0; i < 3; i++) {
                strips[i].clear();

                int up_pos = aniVal / (255 / shouldLightNum);
                for (int j = 0; j < shouldLightNum; j += 4) {
                    double pos_diff =
                        1 - ((double)abs(j - up_pos) / (double)shouldLightNum);

                    color = Adafruit_NeoPixel::Color(
                        green * MAX_BRIGHTNESS * pos_diff,
                        red * MAX_BRIGHTNESS * pos_diff,
                        blue * MAX_BRIGHTNESS * pos_diff);

                    strips[i].setPixelColor(j, color);
                }

                strips[i].show();
            }

            break;

        case DisplayMode::BREATHING:
            if (aniVal < 255 / 2)
                brightness = (double)aniVal / (double)127;
            else
                brightness = 2 - (double)aniVal / (double)127;

            color =
                Adafruit_NeoPixel::Color(red * brightness * MAX_BRIGHTNESS,
                                         green * brightness * MAX_BRIGHTNESS,
                                         blue * brightness * MAX_BRIGHTNESS);

            for (int i = 0; i < 3; i++) {
                strips[i].clear();
                for (int j = 0; j < shouldLightNum; j += 4) {
                    strips[i].setPixelColor(j, color);
                }
                strips[i].show();
            }

            break;

        case DisplayMode::SWIPE_RIGHT:

            // Break ponits between 0 and 255
            int bps[] = {0, 42, 85, 127, 170, 212, 255};

            for (int i = 0; i < 3; i++) {
                int brightness = 0;

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

void State::nextTodo() {
    if (displayTodoIndex == todoAmount - 1) return;
    setDisplayTodoIndex(displayTodoIndex + 1);
}

void State::prevTodo() {
    if (displayTodoIndex == 0) return;
    setDisplayTodoIndex(displayTodoIndex - 1);
}