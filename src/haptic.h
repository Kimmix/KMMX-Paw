const int hapticChannel = 0;
const int motorPin = GPIO_NUM_0;
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