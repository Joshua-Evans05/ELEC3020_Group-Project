// ------------- US Sensor code -----------------

#include <Arduino.h>
#include <TFT_eSPI.h>

const int TRIG_PIN = 2;
const int ECHO_PIN = 1;
const int LED_PIN = 11;

TFT_eSPI tft = TFT_eSPI();

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30 ms timeout
  if (duration == 0) return -1.0;
  return duration * 0.0343 / 2.0;
}

void setup() {
  // Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  tft.init();
  tft.setRotation(1);               // landscape, 240x135
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("HC-SR04 Distance", tft.width() / 2, 6, 4);
  tft.drawFastHLine(10, 34, tft.width() - 20, TFT_DARKGREY);
}

void loop() {
  float cm = readDistanceCm();
  char buf[12];

  tft.setTextDatum(MC_DATUM);
  tft.setTextPadding(tft.width() - 10);   // wipes old text without flicker

  if (cm < 0) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Out of range", tft.width() / 2, 85, 4);
  } else {
    uint16_t colour = (cm > 50) ? TFT_GREEN : (cm > 15) ? TFT_YELLOW : TFT_RED;
    tft.setTextColor(colour, TFT_BLACK);
    snprintf(buf, sizeof(buf), "%.1f", cm);
    tft.drawString(buf, tft.width() / 2 - 15, 85, 6);   // big digits
    tft.setTextPadding(0);
    tft.drawString("cm", tft.width() / 2 + 85, 95, 4);  // unit, smaller font
  }

  if (cm > 0 && cm < 100) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Serial.println(cm);
  delay(150);
}