#include <Arduino.h>
#include <TFT_eSPI.h>

#define AIN1 18
#define AIN2 43
#define PWMA 17

#define BIN1 16
#define BIN2 44
#define PWMB 21

#define LEFT_FRONT_SENSOR 3
#define RIGHT_FRONT_SENSOR 4
#define LEFT_REAR_SENSOR 5
#define RIGHT_REAR_SENSOR 6

TFT_eSPI tft = TFT_eSPI();

const int DRIVE_SPEED = 120;
const int REVERSE_SPEED = 120;
const int TURN_SPEED = 120;

const int REVERSE_DURATION = 500;
const int TURN_DURATION = 500;

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
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);

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

    pinMode(LEFT_FRONT_SENSOR, INPUT);
    pinMode(RIGHT_FRONT_SENSOR, INPUT);
    pinMode(LEFT_REAR_SENSOR, INPUT);
    pinMode(RIGHT_REAR_SENSOR, INPUT);

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
    int frontLeft =
        digitalRead(LEFT_FRONT_SENSOR);

    int frontRight =
        digitalRead(RIGHT_FRONT_SENSOR);

    int rearLeft =
        digitalRead(LEFT_REAR_SENSOR);

    int rearRight =
        digitalRead(RIGHT_REAR_SENSOR);

    if (frontLeft == HIGH &&
        frontRight == HIGH)
    {
        showState("BACK");

        driveBackward(REVERSE_SPEED);
        delay(REVERSE_DURATION);

        turnRight(TURN_SPEED);
        delay(TURN_DURATION);
    }

    else if (frontLeft == HIGH)
    {
        showState("RIGHT");

        driveBackward(REVERSE_SPEED);
        delay(REVERSE_DURATION);

        turnRight(TURN_SPEED);
        delay(TURN_DURATION);
    }

    else if (frontRight == HIGH)
    {
        showState("LEFT");

        driveBackward(REVERSE_SPEED);
        delay(REVERSE_DURATION);

        turnLeft(TURN_SPEED);
        delay(TURN_DURATION);
    }

    else if (rearLeft == HIGH ||
             rearRight == HIGH)
    {
        showState("FORWARD");

        driveForward(DRIVE_SPEED);
        delay(500);
    }

    else
    {
        showState("DRIVE");

        driveForward(DRIVE_SPEED);
    }

    delay(20);
}