// This code is just a test code file for locating an object and then attacking/pushing it

#include <Arduino.h>
#include <TFT_eSPI.h>

// motors
#define AIN1 18
#define AIN2 43
#define PWMA 17

#define BIN1 16
#define BIN2 44
#define PWMB 21

// US
#define TRIG_PIN 2
#define ECHO_PIN 1

#define LED_PIN 11

TFT_eSPI tft = TFT_eSPI();

// change this and calibrate them
const float DETECT_DISTANCE = 80.0; // cm
const float MIN_DISTANCE = 2.0;
const int SEARCH_SPEED = 85;
const int ATTACK_SPEED = 255;

// US sensor reading function

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
    {
        return -1.0;
    }

    return duration * 0.0343 / 2.0;
}

// actual funciton
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
    // L motor forward
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    // R motor back
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);

    analogWrite(PWMA, speed);
    analogWrite(PWMB, speed);
}

void showState(const char *state, float distance)
{
    tft.fillScreen(TFT_BLACK);

    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString(
        state,
        tft.width() / 2,
        tft.height() / 2 - 20,
        4
    );

    if (distance > 0)
    {
        String distanceText = String(distance, 1) + " cm";

        tft.drawString(
            distanceText,
            tft.width() / 2,
            tft.height() / 2 + 25,
            4
        );
    }
}

void setup()
{
    // Motors
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    // US
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(LED_PIN, OUTPUT);

    // TTGO display power
    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    stopMotors();

    showState("READY", -1);

    delay(2000);
}

void loop()
{
    float distance = readDistanceCm();

    if (distance >= MIN_DISTANCE &&
        distance <= DETECT_DISTANCE)
    {
        digitalWrite(LED_PIN, HIGH);

        showState("ATTACK", distance);

        // Drive straight toward object
        driveForward(ATTACK_SPEED);
    }

    else
    {
        digitalWrite(LED_PIN, LOW);

        showState("SEARCH", distance);

        // Rotate until an object enters the ultrasonic sensor field of view
        turnRight(SEARCH_SPEED);
    }

    delay(40);
}