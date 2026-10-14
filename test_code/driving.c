#include <Arduino.h>

const int PWM1 = 44;
const int DIR1 = 43;

const int PWM2 = 17;
const int DIR2 = 18;

void setup()
{
    pinMode(DIR1, OUTPUT);
    pinMode(DIR2, OUTPUT);

    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);

    ledcSetup(0, 5000, 8);
    ledcAttachPin(PWM1, 0);

    ledcSetup(1, 5000, 8);
    ledcAttachPin(PWM2, 1);

    ledcWrite(0, 150);
    ledcWrite(1, 150);
}

void loop()
{
}