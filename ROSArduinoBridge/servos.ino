// servos.cpp
#include "servos.h"

SweepServo servo;  // instantiate

SweepServo::SweepServo()
  : stepDelayMs(0),
    currentPositionDegrees(0),
    targetPositionDegrees(0),
    lastSweepTime(0)
{}

void SweepServo::initServo(int pin, int delayMs, int initPos) {
  servo.attach(pin);
  stepDelayMs = delayMs;
  currentPositionDegrees = initPos;
  targetPositionDegrees = initPos;
  servo.write(initPos);
}

void SweepServo::doSweep() {
  if (currentPositionDegrees == targetPositionDegrees) return;
  unsigned long now = millis();
  if (now - lastSweepTime < (unsigned long)stepDelayMs) return;
  lastSweepTime = now;
  if (currentPositionDegrees < targetPositionDegrees) {
    currentPositionDegrees++;
  } else {
    currentPositionDegrees--;
  }
  servo.write(currentPositionDegrees);
}

void SweepServo::setTargetPosition(int pos) {
  targetPositionDegrees = constrain(pos, 0, 180);
}
