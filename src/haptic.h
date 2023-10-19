const int hapticChannel = 0;
const int motorPin = GPIO_NUM_2;
const unsigned long duration = 40;
void setupMotor() {
    ledcSetup(hapticChannel, 5000, 8);
    // ledcAttachPin(ledPin, hapticChannel);
    ledcAttachPin(motorPin, hapticChannel);
    ledcWrite(hapticChannel, 200);
    delay(100);
    ledcWrite(hapticChannel, 0);
}

void motorVibrate() {
    delay(50);
    ledcWrite(hapticChannel, 255);
    delay(duration);
    ledcWrite(hapticChannel, 0);
}