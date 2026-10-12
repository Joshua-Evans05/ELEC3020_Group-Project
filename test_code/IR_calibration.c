// this is code to test the functionality of the IR sensors and also identify if any calibration is required

#include <Arduino.h>
#include <TFT_eSPI.h>

#define LEFT_FRONT_SENSOR 3
#define RIGHT_FRONT_SENSOR 4
#define LEFT_REAR_SENSOR 5
#define RIGHT_REAR_SENSOR 6

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
        "LF: " + String(leftFront == LOW ? "BLACK" : "WHITE"),
        10,
        10,
        2
    );

    tft.drawString(
        "RF: " + String(rightFront == LOW ? "BLACK" : "WHITE"),
        120,
        10,
        2
    );

    tft.drawString(
        "LR: " + String(leftRear == LOW ? "BLACK" : "WHITE"),
        10,
        60,
        2
    );

    tft.drawString(
        "RR: " + String(rightRear == LOW ? "BLACK" : "WHITE"),
        120,
        60,
        2
    );

    Serial.print("LF: ");
    Serial.print(leftFront == LOW ? "BLACK" : "WHITE");

    Serial.print("  RF: ");
    Serial.print(rightFront == LOW ? "BLACK" : "WHITE");

    Serial.print("  LR: ");
    Serial.print(leftRear == LOW ? "BLACK" : "WHITE");

    Serial.print("  RR: ");
    Serial.println(rightRear == LOW ? "BLACK" : "WHITE");

    delay(100);
}