// motor_driver.h
#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

// Motor indices
#define FL 0
#define FR 1
#define RL 2
#define RR 3

// L298 pin assignments (PWM on all forward/backward lines)
#ifdef L298_MOTOR_DRIVER
  // Front‐Left
  #define FL_FORWARD_PIN   3
  #define FL_BACKWARD_PIN  4
  // Front‐Right
  #define FR_FORWARD_PIN   5
  #define FR_BACKWARD_PIN  6
  // Rear‐Left
  #define RL_FORWARD_PIN   9
  #define RL_BACKWARD_PIN 10
  // Rear‐Right
  #define RR_FORWARD_PIN  11
  #define RR_BACKWARD_PIN 12
#endif

// Initialize all motor‐driver pins
void initMotorController();

// Set one wheel’s speed: idx = FL|FR|RL|RR, spd = –255…+255
void setMotorSpeed(uint8_t idx, int spd);

// Set all four wheel speeds at once
void setMotorSpeeds(int flSpeed, int frSpeed, int rlSpeed, int rrSpeed);

#endif // MOTOR_DRIVER_H
