// motor_driver.ino
#include "motor_driver.h"

#ifdef L298_MOTOR_DRIVER

void initMotorController() {
  // configure all four motors’ direction pins as outputs
  pinMode(FL_FORWARD_PIN,  OUTPUT);
  pinMode(FL_BACKWARD_PIN, OUTPUT);
  pinMode(FR_FORWARD_PIN,  OUTPUT);
  pinMode(FR_BACKWARD_PIN, OUTPUT);
  pinMode(RL_FORWARD_PIN,  OUTPUT);
  pinMode(RL_BACKWARD_PIN, OUTPUT);
  pinMode(RR_FORWARD_PIN,  OUTPUT);
  pinMode(RR_BACKWARD_PIN, OUTPUT);
}

// helper to drive one motor
static void driveMotorPins(uint8_t fp, uint8_t bp, int spd) {
  bool reverse = false;
  if (spd < 0) { reverse = true; spd = -spd; }
  if (spd > 255) spd = 255;

  if (!reverse) {
    analogWrite(fp, spd);
    analogWrite(bp, 0);
  } else {
    analogWrite(fp, 0);
    analogWrite(bp, spd);
  }
}

void setMotorSpeed(uint8_t idx, int spd) {
  switch (idx) {
    case FL: driveMotorPins(FL_FORWARD_PIN,  FL_BACKWARD_PIN,  spd); break;
    case FR: driveMotorPins(FR_FORWARD_PIN,  FR_BACKWARD_PIN,  spd); break;
    case RL: driveMotorPins(RL_FORWARD_PIN,  RL_BACKWARD_PIN,  spd); break;
    case RR: driveMotorPins(RR_FORWARD_PIN,  RR_BACKWARD_PIN,  spd); break;
    default: /* invalid index */ break;
  }
}

void setMotorSpeeds(int flSpeed, int frSpeed, int rlSpeed, int rrSpeed) {
  setMotorSpeed(FL, flSpeed);
  setMotorSpeed(FR, frSpeed);
  setMotorSpeed(RL, rlSpeed);
  setMotorSpeed(RR, rrSpeed);
}

#endif // L298_MOTOR_DRIVER
