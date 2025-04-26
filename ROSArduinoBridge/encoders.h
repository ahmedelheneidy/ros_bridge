// encoders.h
#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>

// Wheel indices — keep these in sync with your motor driver!
#define FL 0
#define FR 1
#define RL 2
#define RR 3

#ifdef ARDUINO_ENC_COUNTER
  // A-channel pins (must be interrupt-capable)
  #define FL_ENC_A_PIN  2   // INT0
  #define FR_ENC_A_PIN  3   // INT1
  #define RL_ENC_A_PIN 18   // INT5
  #define RR_ENC_A_PIN 19   // INT4

  // B-channel pins (any digital pins)
  #define FL_ENC_B_PIN 22
  #define FR_ENC_B_PIN 23
  #define RL_ENC_B_PIN 24
  #define RR_ENC_B_PIN 25
#endif

// Call once from your main setup()
void initEncoders();

long readEncoder(int wheel);
void resetEncoder(int wheel);
void resetEncoders();

#endif // ENCODERS_H
