#include <Arduino.h>
#include <TFT_eSPI.h>

// motors - Cytron MDD10A
#define PWM1 44
#define DIR1 43

#define PWM2 17
#define DIR2 18

// ultrasonic sensor
#define TRIG_PIN 2
#define ECHO_PIN 1

// infrared sensors
#define LEFT_FRONT_SENSOR 11
#define RIGHT_FRONT_SENSOR 12
#define LEFT_REAR_SENSOR 10
#define RIGHT_REAR_SENSOR 13

TFT_eSPI tft = TFT_eSPI();


// ultrasonic distances
const float DETECT_DISTANCE = 80.0;
const float FULL_ATTACK_DISTANCE = 30.0;
const float MIN_DISTANCE = 2.0;


// motor speeds
const int SEARCH_SPEED = 100;
const int NORMAL_SPEED = 180;
const int FULL_SPEED = 255;


// movement times
const int REVERSE_TIME = 500;
const int SIDE_TURN_TIME = 700;
const int TURN_180_TIME = 900;
const int REAR_ESCAPE_TIME = 500;


// ultrasonic sensor

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


// motors

void stopMotors()
{
    ledcWrite(0, 0);
    ledcWrite(1, 0);
}


void driveForward(int speed)
{
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);

    ledcWrite(0, speed);
    ledcWrite(1, speed);
}


void driveBackward(int speed)
{
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, LOW);

    ledcWrite(0, speed);
    ledcWrite(1, speed);
}


void turnRight(int speed)
{
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);

    ledcWrite(0, speed);
    ledcWrite(1, speed);
}


void turnLeft(int speed)
{
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);

    ledcWrite(0, speed);
    ledcWrite(1, speed);
}


// display

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
        String distanceText =
            String(distance, 1) + " cm";

        tft.drawString(
            distanceText,
            tft.width() / 2,
            tft.height() / 2 + 25,
            4
        );
    }
}


// boundary detection

bool checkBoundary()
{
    int leftFront =
        digitalRead(LEFT_FRONT_SENSOR);

    int rightFront =
        digitalRead(RIGHT_FRONT_SENSOR);

    int leftRear =
        digitalRead(LEFT_REAR_SENSOR);

    int rightRear =
        digitalRead(RIGHT_REAR_SENSOR);


    // both rear sensors detect black
    if (leftRear == HIGH &&
        rightRear == HIGH)
    {
        showState("REAR EDGE", -1);

        driveForward(FULL_SPEED);
        delay(REAR_ESCAPE_TIME);

        stopMotors();
        delay(100);

        return true;
    }


    // both front sensors detect black
    if (leftFront == HIGH &&
        rightFront == HIGH)
    {
        showState("FRONT EDGE", -1);

        stopMotors();
        delay(100);

        driveBackward(NORMAL_SPEED);
        delay(REVERSE_TIME);

        stopMotors();
        delay(100);

        turnRight(NORMAL_SPEED);
        delay(TURN_180_TIME);

        stopMotors();
        delay(100);

        return true;
    }


    // left side detects black
    if (leftFront == HIGH)
    {
        showState("LEFT EDGE", -1);

        // driveBackward(SEARCH_SPEED);
        // delay(200);
        turnLeft(NORMAL_SPEED);
        delay(1500);
        driveForward(SEARCH_SPEED);
        delay(200);

        stopMotors();
        delay(100);

        return true;
    }

    if (leftRear == HIGH)
    {
        showState("LEFT EDGE", -1);

        // driveForward(SEARCH_SPEED);
        // delay(200);
        turnLeft(SEARCH_SPEED);
        delay(900);
        driveForward(SEARCH_SPEED);
        delay(200);

        stopMotors();
        delay(100);

        return true;
    }

    // right side detects black
    if (rightFront == HIGH )
    {
        showState("RIGHT EDGE", -1);

        // driveBackward(SEARCH_SPEED);
        // delay(900);
        turnRight(NORMAL_SPEED);
        delay(500);
        driveForward(SEARCH_SPEED);
        delay(200);

        stopMotors();
        delay(100);

        return true;
    }

    if (rightRear == HIGH )
    {
        showState("RIGHT EDGE", -1);

        turnRight(SEARCH_SPEED);
        delay(900);
        driveForward(SEARCH_SPEED);
        delay(200);

        stopMotors();
        delay(100);

        return true;
    }


    return false;
}


void setup()
{
    Serial.begin(115200);

    // motor direction pins
    pinMode(DIR1, OUTPUT);
    pinMode(DIR2, OUTPUT);

    // PWM
    ledcSetup(0, 5000, 8);
    ledcAttachPin(PWM1, 0);

    ledcSetup(1, 5000, 8);
    ledcAttachPin(PWM2, 1);

    // ultrasonic
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // infrared sensors
    pinMode(LEFT_FRONT_SENSOR, INPUT);
    pinMode(RIGHT_FRONT_SENSOR, INPUT);
    pinMode(LEFT_REAR_SENSOR, INPUT);
    pinMode(RIGHT_REAR_SENSOR, INPUT);

    // TTGO display
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
    // boundary detection always has highest priority
    if (checkBoundary())
    {
        return;
    }


    float distance = readDistanceCm();


    // opponent very close - maximum power
    if (distance >= MIN_DISTANCE &&
        distance <= FULL_ATTACK_DISTANCE)
    {
        showState("FULL ATTACK", distance);

        driveForward(FULL_SPEED);
    }


    // opponent detected - normal attack
    else if (distance > FULL_ATTACK_DISTANCE &&
             distance <= DETECT_DISTANCE)
    {
        showState("ATTACK", distance);

        driveForward(NORMAL_SPEED);
    }


    // no opponent - search
    else
    {
        showState("SEARCH", distance);

        turnRight(SEARCH_SPEED);
    }


    delay(40);
}