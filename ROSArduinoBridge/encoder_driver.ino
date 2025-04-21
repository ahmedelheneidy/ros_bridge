#include "encoder_driver.h"
#include <Arduino.h>

volatile long encCounts[4] = {0,0,0,0};

void handleFLA() { encCounts[FL] += (digitalRead(FL_ENC_PIN_B) ? +1 : -1); }
void handleFLB() { encCounts[FL] += (digitalRead(FL_ENC_PIN_A) ? -1 : +1); }
void handleFRA() { encCounts[FR] += (digitalRead(FR_ENC_PIN_B) ? +1 : -1); }
void handleFRB() { encCounts[FR] += (digitalRead(FR_ENC_PIN_A) ? -1 : +1); }
void handleRLA() { encCounts[RL] += (digitalRead(RL_ENC_PIN_B) ? +1 : -1); }
void handleRLB() { encCounts[RL] += (digitalRead(RL_ENC_PIN_A) ? -1 : +1); }
void handleRRA() { encCounts[RR] += (digitalRead(RR_ENC_PIN_B) ? +1 : -1); }
void handleRRB() { encCounts[RR] += (digitalRead(RR_ENC_PIN_A) ? -1 : +1); }

void setupEncoders() {
  pinMode(FL_ENC_PIN_A, INPUT_PULLUP);
  pinMode(FL_ENC_PIN_B, INPUT_PULLUP);
  pinMode(FR_ENC_PIN_A, INPUT_PULLUP);
  pinMode(FR_ENC_PIN_B, INPUT_PULLUP);
  pinMode(RL_ENC_PIN_A, INPUT_PULLUP);
  pinMode(RL_ENC_PIN_B, INPUT_PULLUP);
  pinMode(RR_ENC_PIN_A, INPUT_PULLUP);
  pinMode(RR_ENC_PIN_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(FL_ENC_PIN_A), handleFLA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FL_ENC_PIN_B), handleFLB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FR_ENC_PIN_A), handleFRA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FR_ENC_PIN_B), handleFRB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RL_ENC_PIN_A), handleRLA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RL_ENC_PIN_B), handleRLB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RR_ENC_PIN_A), handleRRA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RR_ENC_PIN_B), handleRRB, CHANGE);
}

long readEncoder(int i) {
  noInterrupts();
  long v = encCounts[i];
  interrupts();
  return v;
}

void resetEncoder(int i) {
  noInterrupts();
  encCounts[i] = 0;
  interrupts();
}

void resetEncoders() {
  for (int i = 0; i < 4; i++) resetEncoder(i);
}
