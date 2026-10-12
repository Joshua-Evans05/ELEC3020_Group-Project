// this is a very basic code to test the functionality and calibration of the US sensors

#include <Arduino.h>
#include <TFT_eSPI.h>

#define TRIG_PIN 2
#define ECHO_PIN 1

TFT_eSPI tft = TFT_eSPI();

float readDistanceCm()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0)
    {
        return -1.0;
    }

    return duration * 0.0343 / 2.0;
}

void setup()
{
    Serial.begin(115200);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    tft.setTextDatum(MC_DATUM);
}

void loop()
{
    float distance = readDistanceCm();

    Serial.print("Distance: ");
    Serial.print(distance, 1);
    Serial.println(" cm");

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    if (distance > 0)
    {
        String text = String(distance, 1) + " cm";

        tft.drawString(
            text,
            tft.width() / 2,
            tft.height() / 2,
            4
        );
    }
    else
    {
        tft.drawString(
            "NO READING",
            tft.width() / 2,
            tft.height() / 2,
            4
        );
    }

    delay(200);
}