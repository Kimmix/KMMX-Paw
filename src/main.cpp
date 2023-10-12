#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Adafruit_MPR121.h"

const int hapticChannel = 0;
const int motorPin = GPIO_NUM_0;
// const int ledPin = LED_BUILTIN;
void setupMotor() {
    ledcSetup(hapticChannel, 5000, 8);
    // ledcAttachPin(ledPin, hapticChannel);
    ledcAttachPin(motorPin, hapticChannel);
}
unsigned long currentMillis;
unsigned long previousMillis;
unsigned long duration = 70;

void motorVibrate() {
    ledcWrite(hapticChannel, 255);
    previousMillis = millis();  // Store the current time
}

void turnOffMotor() {
    if (millis() - previousMillis >= duration) {
        ledcWrite(hapticChannel, 0);
    }
}

uint8_t broadcastAddress[] = {0x40, 0x22, 0xD8, 0x4C, 0xAB, 0x98};
static const char* PMK_KEY_STR = "NHkeBaL5YkoAUsi6";
static const char* LMK_KEY_STR = "eYF8CUjnkFq3Ke5f";

typedef struct struct_message {
    uint16_t debugLed;
    int b;
} struct_message;
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setupEspNow() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Set PMK key
    esp_now_set_pmk((uint8_t*)PMK_KEY_STR);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = true;
    // Set the receiver device LMK key
    for (uint8_t i = 0; i < 16; i++) {
        peerInfo.lmk[i] = LMK_KEY_STR[i];
    }

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
}
// ------------------------------- MPR121
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t currtouched = 0;
uint16_t prevState = 0;
int state = 0;
void setupMPR121() {
    if (!cap.begin(0x5A)) {
        Serial.println("Couldn't setup MPR121");
        while (1)
            ;
    }
    Serial.println("MPR121 found!");
}

void scanMPR() {
    currtouched = cap.touched();
    switch (currtouched) {
        case 1 << 0:
            state = 1;
            break;
        case 1 << 1:
            state = 2;
            break;
        case 1 << 2:
            state = 3;
            break;
        case 1 << 3:
            state = 4;
            break;
        case 1 << 4:
            state = 5;
            break;
        case 1 << 5:
            state = 6;
            break;
        default:
            state = 0;
            break;
    }
}

void sendEspnow() {
    myData.b = state;
    myData.debugLed = currtouched;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }

    if (result == ESP_OK && state != 0) {
        motorVibrate();
    }
}

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