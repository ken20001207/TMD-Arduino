#include "global.h"
#include "model.h"

SoftwareSerial espSerial(2, 3);

State state = State();

void setup() {
    Serial.begin(9600);
    espSerial.begin(4800);
    lcd.begin(16, 2);
    paj7620Init();
    printTextLCD(lcd, "Too Many DDL 1.0", "By Yuanlin Lin");
    requstData();
}

void requstData() {
    printTextLCD(lcd, "Fetching data ...", "Please wait");
    // Send a char to requset ESP8266 fetch data
    espSerial.print("a");
}

StaticJsonDocument<300> jsonDoc;
DeserializationError error;

uint8_t gesture = 0;
int code;
char *ssid;

void loop() {
    if (espSerial.available()) {
        deserializeJson(jsonDoc, espSerial);

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
                        char *id = jsonDoc["data"][i]["id"];
                        char *title = jsonDoc["data"][i]["title"];
                        char *ddl = jsonDoc["data"][i]["ddl"];
                        state.setTodo(i, id, title, ddl);
                    }
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
                state.setDisplayTodoIndex(0);
                break;
            case GES_LEFT_FLAG:
                state.setDisplayTodoIndex(1);
                break;
            case GES_DOWN_FLAG:
                requstData();
                break;
            default:
                break;
        }
    } else {
        paj7620Init();
    }
}