#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Adafruit_MPR121.h"
#include "haptic.h"
#include "mpr121.h"
#include "espnow.h"

enum class State { ENABLE,
                   DISABLE };
State pawControl = State::ENABLE;

void setup() {
    // Serial.begin(115200);
    // delay(1000);
    setupMotor();
    setupEspNow();
    setupMPR121();
}
void loop() {
    scanMPR();
    if (currtouched != prevState) {
        Serial.println(currtouched, BIN);
        sendEspnow();
        delay(100);
        prevState = currtouched;
    }
    // switch (pawControl) {
    //     case State::ENABLE:
    //         if (currtouched != prevState) {
    //             // Serial.println(currtouched, BIN);
    //             sendEspnow();
    //             prevState = currtouched;
    //         }
    //         break;
    //     case State::DISABLE:
    //         break;
    // }
}