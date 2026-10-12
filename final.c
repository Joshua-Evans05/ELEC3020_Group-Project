#include <Arduino.h>
#include <TFT_eSPI.h>

#define AIN1 18
#define AIN2 43
#define PWMA 17

#define BIN1 16
#define BIN2 44
#define PWMB 21

// US sensors

#define TRIG_PIN 2
#define ECHO_PIN 1

// IR sensors

#define LEFT_FRONT_SENSOR 3
#define RIGHT_FRONT_SENSOR 4
#define LEFT_REAR_SENSOR 5
#define RIGHT_REAR_SENSOR 6

// LED
#define LED_PIN 11

TFT_eSPI tft = TFT_eSPI();

const float DETECT_DISTANCE = 100.0;

const int SEARCH_SPEED = 90;
const int ATTACK_SPEED = 150;
const int REVERSE_SPEED = 100;
const int TURN_SPEED = 110;

// Tune these
const int REVERSE_DURATION = 500;
const int FORWARD_ESCAPE_DURATION = 500;
const int TURN_DURATION = 400;


// US sensors

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

// Motor controls

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


void driveBackward(int speed)
{
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);

    analogWrite(PWMA, speed);
    analogWrite(PWMB, speed);
}


void turnLeft(int speed)
{
    // Left wheel backward
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);

    // Right wheel forward
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);

    analogWrite(PWMA, speed);
    analogWrite(PWMB, speed);
}


void turnRight(int speed)
{
    // Left wheel forward
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    // Right wheel backward
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

void checkBoundary()
{
    int frontLeft =
        digitalRead(LEFT_FRONT_SENSOR);

    int frontRight =
        digitalRead(RIGHT_FRONT_SENSOR);

    int rearLeft =
        digitalRead(LEFT_REAR_SENSOR);

    int rearRight =
        digitalRead(RIGHT_REAR_SENSOR);

    if (frontLeft == HIGH)
    {
        showState("ESCAPE");

        driveBackward(REVERSE_SPEED);
        delay(REVERSE_DURATION);

        turnRight(TURN_SPEED);
        delay(TURN_DURATION);

        stopMotors();

        return;
    }

    if (frontRight == HIGH)
    {
        showState("ESCAPE");

        driveBackward(REVERSE_SPEED);
        delay(REVERSE_DURATION);

        turnLeft(TURN_SPEED);
        delay(TURN_DURATION);

        stopMotors();

        return;
    }

    if (frontLeft == HIGH &&
        frontRight == HIGH)
    {
        showState("ESCAPE");

        driveBackward(REVERSE_SPEED);

        delay(REVERSE_DURATION);

        turnRight(TURN_SPEED);

        delay(TURN_DURATION);

        stopMotors();

        return;
    }

    if (rearLeft == HIGH ||
        rearRight == HIGH)
    {
        showState("ESCAPE");

        driveForward(REVERSE_SPEED);

        delay(FORWARD_ESCAPE_DURATION);

        turnRight(TURN_SPEED);

        delay(TURN_DURATION);

        stopMotors();

        return;
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

    // IR
    pinMode(LEFT_FRONT_SENSOR, INPUT);
    pinMode(RIGHT_FRONT_SENSOR, INPUT);
    pinMode(LEFT_REAR_SENSOR, INPUT);
    pinMode(RIGHT_REAR_SENSOR, INPUT);

    // LED
    pinMode(LED_PIN, OUTPUT);

    // Displays the  power
    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    tft.init();
    tft.setRotation(3);

    tft.fillScreen(TFT_BLACK);

    showState("READY");

    stopMotors();

    delay(2000);
}

void loop()
{
    int frontLeft =
        digitalRead(LEFT_FRONT_SENSOR);

    int frontRight =
        digitalRead(RIGHT_FRONT_SENSOR);

    int rearLeft =
        digitalRead(LEFT_REAR_SENSOR);

    int rearRight =
        digitalRead(RIGHT_REAR_SENSOR);


    if (frontLeft == HIGH ||
        frontRight == HIGH ||
        rearLeft == HIGH ||
        rearRight == HIGH)
    {
        checkBoundary();

        return;
    }

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