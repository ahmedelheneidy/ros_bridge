// servos.h
#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <Servo.h>

// Single‐servo settings
static const int STEP_DELAY_MS    = 0;    // ms between degree‐steps
static const byte SERVO_PIN       = 29;   // control pin
static const byte INIT_POSITION   = 90;   // [0..180]°

class SweepServo {
public:
  SweepServo();
  void initServo(int servoPin, int stepDelayMs, int initPosition);
  void doSweep();
  void setTargetPosition(int position);
private:
  Servo        servo;
  int          stepDelayMs;
  int          currentPositionDegrees;
  int          targetPositionDegrees;
  unsigned long lastSweepTime;
};

// single instance
extern SweepServo servo;

#endif // SERVOS_H
