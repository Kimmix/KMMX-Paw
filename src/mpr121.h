// ------------------------------- MPR121
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t currtouched = 0;
uint16_t prevState = 0;
int state = 0;
void setupMPR121() {
    if (!cap.begin(0x5A)) {
        Serial.println("Couldn't setup MPR121");
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