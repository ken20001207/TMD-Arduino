#include "global.h"
#include "model.h"

State state = State();

void setup() {
    Serial.begin(9600);
    Serial1.begin(4800);

    lcd.begin(16, 2);
    printTextLCD(lcd, "Too Many DDL 1.0", "By Yuanlin Lin");

    paj7620Init();

    requstData();

    for (int i = 0; i < 3; i++) {
        strips[i].begin();
    }
}

void requstData() {
    printTextLCD(lcd, "Fetching data ...", "Please wait");

    state.setLEDMode(DisplayMode::BREATHING);
    state.setShouldLightNum(40);
    state.setDisplayColor(255, 255, 255);

    // Send a char to requset ESP8266 fetch data
    Serial1.print("a");
}

StaticJsonDocument<300> jsonDoc;
DeserializationError error;

uint8_t gesture = 0;
int code;
char *ssid;
char *msg;
char *status;

void loop() {
    state.addAniVal();

    if (Serial1.available()) {
        deserializeJson(jsonDoc, Serial1);

        if (!error) {
            code = jsonDoc["code"];
            switch (code) {
                case EspResponse::CONNECTING_WIFI:
                    printTextLCD(lcd, "Connecting WIFI", "Please Wait ...");
                    break;

                case EspResponse::WIFI_CONNECTED:
                    ssid = jsonDoc["ssid"];
                    printTextLCD(lcd, "Connected to", ssid);
                    delay(1000);
                    requstData();
                    break;

                case EspResponse::FETCH_SUCCESS:
                    int todoAmount = jsonDoc["data"].size();

                    Serial.print("Total ");
                    Serial.print(todoAmount);
                    Serial.println(" todo data fetched.");

                    for (int i = 0; i < todoAmount && i < 10; i++) {
                        char *title = jsonDoc["data"][i]["title"];
                        char *ddl = jsonDoc["data"][i]["ddl"];
                        char *status = jsonDoc["data"][i]["status"];
                        state.setTodo(i, title, ddl, status);
                    }
                    state.setDisplayTodoIndex(0);
                    break;

                case EspResponse::REQUEST_FAILED:
                    msg = jsonDoc["msg"];
                    Serial.println(msg);
                    break;

                case EspResponse::HTTP_NOT_OK:
                    msg = jsonDoc["msg"];
                    Serial.println(msg);
                    break;

                default:
                    break;
            }

        } else {
            Serial.println(error.c_str());
        }
    }

    uint8_t error = paj7620ReadReg(0x43, 1, &gesture);

    if (!error) {
        switch (gesture) {
            case GES_RIGHT_FLAG:
                Serial.println("[Gesture] Get GES_RIGHT_FLAG");
                state.nextTodo();
                break;
            case GES_LEFT_FLAG:
                Serial.println("[Gesture] Get GES_LEFT_FLAG");
                state.prevTodo();
                break;
            case GES_DOWN_FLAG:
                Serial.println("[Gesture] Get GES_DOWN_FLAG");
                requstData();
                break;
            default:
                break;
        }
    } else {
        paj7620Init();
    }
}