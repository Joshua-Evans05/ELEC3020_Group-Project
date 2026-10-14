// this is code to test the functionality of the IR sensors and also identify if any calibration is required

#include <Arduino.h>
#include <TFT_eSPI.h>

#define LEFT_FRONT_SENSOR 11
#define RIGHT_FRONT_SENSOR 12
#define LEFT_REAR_SENSOR 13
#define RIGHT_REAR_SENSOR 10

TFT_eSPI tft = TFT_eSPI();

void setup()
{
    Serial.begin(115200);

    pinMode(LEFT_FRONT_SENSOR, INPUT);
    pinMode(RIGHT_FRONT_SENSOR, INPUT);
    pinMode(LEFT_REAR_SENSOR, INPUT);
    pinMode(RIGHT_REAR_SENSOR, INPUT);

    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
}

void loop()
{
    int leftFront = digitalRead(LEFT_FRONT_SENSOR);
    int rightFront = digitalRead(RIGHT_FRONT_SENSOR);
    int leftRear = digitalRead(LEFT_REAR_SENSOR);
    int rightRear = digitalRead(RIGHT_REAR_SENSOR);

    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString(
        "LF: " + String(leftFront),
        10,
        10,
        2
    );

    tft.drawString(
        "RF: " + String(rightFront),
        120,
        10,
        2
    );

    tft.drawString(
        "LR: " + String(leftRear),
        10,
        60,
        2
    );

    tft.drawString(
        "RR: " + String(rightRear),
        120,
        60,
        2
    );

    Serial.print("LF: ");
    Serial.print(leftFront);

    Serial.print("  RF: ");
    Serial.print(rightFront);

    Serial.print("  LR: ");
    Serial.print(leftRear);

    Serial.print("  RR: ");
    Serial.println(rightRear);

    delay(100);
}