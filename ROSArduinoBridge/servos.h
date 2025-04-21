#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <Servo.h>

#define N_SERVOS 2
extern int stepDelay[N_SERVOS];
extern byte servoPins[N_SERVOS];
extern byte servoInitPosition[N_SERVOS];

class SweepServo {
  public:
    SweepServo();
    void initServo(int servoPin, int stepDelayMs, int initPosition);
    void doSweep();
    void setTargetPosition(int position);
  private:
    Servo servo;
    int stepDelayMs;
    int currentPositionDegrees;
    int targetPositionDegrees;
    unsigned long lastSweepTime;
};

extern SweepServo servos[N_SERVOS];

#endif
