/* #include <Arduino.h>

// Motor A Pins
#define AIN1 18
#define AIN2 43
#define PWMA 17

// Motor B Pins
#define BIN1 16
#define BIN2 44
#define PWMB 21



void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  
}

void loop() {

  delay(2000); // Wait for 2 seconds before starting the motor sequence
  
  // Both motors forward (~75% speed)
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 255);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 255);
  
  delay(2000);

  // Stop both motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  
  delay(1000);

  // Both motors backward (~75% speed)
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 255);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 255);
  
  delay(2000);

  // Stop both motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  
  delay(1000);
}



 #include <Arduino.h>

// Ultrasonic Sensor Pins (Top Header)
#define TRIG_PIN 12
#define ECHO_PIN 13

void setup() {
  // Initialize Serial Communication at 115200 baud
  Serial.begin(115200);

  // Configure Ultrasonic Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.println("--- Ultrasonic Distance Monitor Initialized ---");
}

float getDistanceCM() {
  // Send 10-microsecond HIGH pulse to trigger sonar
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read return pulse travel time in microseconds
  // Timeout set to 25,000 us (~4.2 meters maximum range)
  long duration = pulseIn(ECHO_PIN, HIGH, 25000);

  if (duration == 0) {
    return -1.0; // Out of range / No echo received
  }

  // Calculate distance in cm (Speed of sound = 0.0343 cm/us, divided by 2 for round-trip)
  return (duration * 0.0343) / 2.0;
}

void loop() {
  float distance = getDistanceCM();

  if (distance < 0) {
    Serial.println("Distance: Out of range");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance, 1); // 1 decimal place
    Serial.println(" cm");
  }

  delay(100); // Sample every 100 milliseconds
} */

// THe new code below should be roughyl 1 metre

#include <Arduino.h>

// Motor A pins
#define AIN1 18
#define AIN2 43
#define PWMA 17

// Motor B pins
#define BIN1 16
#define BIN2 44
#define PWMB 21

void setup()
{
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    delay(2000);


    // forward
  
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);

    analogWrite(PWMA, 100);
    analogWrite(PWMB, 100);

    delay(5500);   // change this

    // stop
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);

    delay(2000);

  
    // reverse
  
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);

    analogWrite(PWMA, 103);
    analogWrite(PWMB, 100);

    delay(5575); // change this

    // stop
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

void loop()
{
    // Do nothing after demonstration
}



// #include <Arduino.h>
// #include <TFT_eSPI.h>

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