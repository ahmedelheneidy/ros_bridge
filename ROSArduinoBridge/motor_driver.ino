#include "motor_driver.h"
#include <Arduino.h>

void initMotorController() {
  const int pins[] = {
    FL_IN1, FL_IN2,
    FR_IN1, FR_IN2,
    RL_IN1, RL_IN2,
    RR_IN1, RR_IN2
  };
  for (int i = 0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
}

void setMotorSpeed(int i, int spd) {
  int in1, in2;
  switch (i) {
    case FL: in1 = FL_IN1; in2 = FL_IN2; break;
    case FR: in1 = FR_IN1; in2 = FR_IN2; break;
    case RL: in1 = RL_IN1; in2 = RL_IN2; break;
    case RR: in1 = RR_IN1; in2 = RR_IN2; break;
    default: return;
  }
  if (spd >= 0) {
    analogWrite(in1, spd);
    digitalWrite(in2, LOW);
  } else {
    analogWrite(in2, -spd);
    digitalWrite(in1, LOW);
  }
}

void setMecanum(int vx, int vy, int omega) {
  // basic mixing: vx=forward, vy=strafe right, omega=rotate CW
  int fl =  vx + vy + omega;
  int fr =  vx - vy - omega;
  int rl =  vx - vy + omega;
  int rr =  vx + vy - omega;

  // optional: scale so max(|fl|,..) ≤ 255

  setMotorSpeed(FL, fl);
  setMotorSpeed(FR, fr);
  setMotorSpeed(RL, rl);
  setMotorSpeed(RR, rr);
}
