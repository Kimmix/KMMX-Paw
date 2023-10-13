#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Adafruit_MPR121.h"
#include "haptic.h"
#include "mpr121.h"
#include "espnow.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(500);
    setupMotor();
    setupMPR121();
    setupEspNow();
}
void loop() {
    scanMPR();
    if (currtouched != prevState) {
        Serial.println(currtouched, BIN);
        sendEspnow();
        prevState = currtouched;
    }
    turnOffMotor();
}