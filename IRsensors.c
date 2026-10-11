#include <Arduino.h>
#include <TFT_eSPI.h>

// vcc = 3.3v
// gnd = gnd
// out = 2

TFT_eSPI tft = TFT_eSPI();

#define IR_SENSOR 2

void setup() {
  pinMode(IR_SENSOR, INPUT);

  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
}

void loop() {

  int sensorValue = digitalRead(IR_SENSOR);

  tft.fillScreen(TFT_BLACK);

  if (sensorValue == LOW) {
    // black detected
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("STOP", tft.width() / 2, tft.height() / 2);
  }
  else {
    // white detected
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("GO", tft.width() / 2, tft.height() / 2);
  }

  delay(100);
}