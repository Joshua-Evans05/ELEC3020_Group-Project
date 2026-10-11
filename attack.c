#include <Arduino.h>
#include <TFT_eSPI.h>

#define AIN1 18
#define AIN2 43
#define PWMA 17

#define BIN1 16
#define BIN2 44
#define PWMB 21

#define TRIG_PIN 2
#define ECHO_PIN 1

#define LED_PIN 11

TFT_eSPI tft = TFT_eSPI();

const float DETECT_DISTANCE = 80.0;

const int SEARCH_SPEED = 90;
const int ATTACK_SPEED = 255;

float readDistanceCm()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration =
        pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0)
        return -1.0;

    return duration * 0.0343 / 2.0;
}

void stopMotors()
{
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

void driveForward(int speed)
{
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);

    analogWrite(PWMA, speed);
    analogWrite(PWMB, speed);
}

void turnRight(int speed)
{
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);

    analogWrite(PWMA, speed);
    analogWrite(PWMB, speed);
}

void showState(String state)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString(
        state,
        tft.width() / 2,
        tft.height() / 2,
        4
    );
}

void setup()
{
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(LED_PIN, OUTPUT);

    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    tft.init();
    tft.setRotation(3);

    showState("READY");

    stopMotors();

    delay(2000);
}

void loop()
{
    float distance = readDistanceCm();

    if (distance > 0 &&
        distance < DETECT_DISTANCE)
    {
        digitalWrite(LED_PIN, HIGH);

        showState("ATTACK");

        driveForward(ATTACK_SPEED);
    }
    else
    {
        digitalWrite(LED_PIN, LOW);

        showState("SEARCH");

        turnRight(SEARCH_SPEED);
    }

    delay(30);
}