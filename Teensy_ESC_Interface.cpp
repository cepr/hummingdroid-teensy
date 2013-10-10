#include "Arduino.h"

void setup();
void loop();

const int ESC_FREQ = 400;
const int MIN = 9 * ESC_FREQ;
const int PINS[4] = {21, 4, 3, 22};
const int MAX_SONAR_PIN = 23;
const unsigned long USB_TIMEOUT_MS = 500;

void setup()
{
    // Configure PWM
    analogWriteResolution(13);
    for (int i = 0; i < 4; i++) {
        pinMode(PINS[i], OUTPUT);
        analogWriteFrequency(PINS[i], ESC_FREQ);
        analogWrite(PINS[i], MIN);
    }

    // Configure Max sonar
    pinMode(MAX_SONAR_PIN, INPUT);

    // Configure Serial
    Serial.begin(9600);
}

void loop()    
{
    static int index = 0;
    static uint8_t previous_pulse_state = LOW;
    static unsigned long pulse_start = 0;
    static unsigned long usb_timestamp = 0;

    // Read serial command from USB
    while (Serial.available()) {
        int b = Serial.read();
        analogWrite(PINS[index], MIN + b * 8);
        index = (index + 1) & 3;
        usb_timestamp = millis();
    }

    // Check for USB timeout
    if (millis() - usb_timestamp > USB_TIMEOUT_MS) {
        analogWrite(PINS[0], MIN);
        analogWrite(PINS[1], MIN);
        analogWrite(PINS[2], MIN);
        analogWrite(PINS[3], MIN);
    }
/*
    // Read MAX SONAR pulse
    uint8_t pulse_state = digitalRead(MAX_SONAR_PIN);
    if (pulse_state != previous_pulse_state) {
        if (previous_pulse_state == LOW) {
            // Pulse is starting
            pulse_start = micros();
        } else {
            // Pulse is finished
            unsigned long duration = micros() - pulse_start;
            if (Serial.dtr()) {
                Serial.write((const uint8_t*)&duration, sizeof(unsigned long));
                Serial.send_now();
            }
        }
        previous_pulse_state = pulse_state;
    }
*/
}

