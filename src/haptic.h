#include "Haptic_DRV2605.h"
Haptic_DRV2605 haptic;
const int startupEffectNo = 52;
const int effectNo = 14;
void setupMotor() {
    // initialize the Haptic controller
    if (haptic.begin() != HAPTIC_SUCCESS) {
        Serial.println("Haptic: Driver Error: DRV2605L Device not found - check your I2C connections?");
    } else {
        // okay correct chip, let's initialize it
        Serial.println("Haptic: DeviceID = DRV2605L @ Address 0x5A was found! ");
        // config the DRV2605 chip
        haptic.setActuatorType(LRA);    // pick an actuator type
        haptic.setMode(REGISTER_MODE);  // haptic effects triggered by I2C register write
    }
    haptic.setWaveform(0, startupEffectNo);  // set the first sequence
    haptic.goWait();                  // play the waveform
    haptic.setWaveform(0, effectNo);  // set the first sequence
}

void motorVibrate() {
    haptic.goWait();  // play the waveform
}