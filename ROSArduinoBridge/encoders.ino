// encoders.ino
#include "encoders.h"

#ifdef ARDUINO_ENC_COUNTER

// four volatile counters
volatile long encCount[4] = {0, 0, 0, 0};

// ——— forward‐declare ISRs ———
void ISR_FL();
void ISR_FR();
void ISR_RL();
void ISR_RR();

// must be called from your sketch’s setup():
void initEncoders() {
  // configure A & B pins
  pinMode(FL_ENC_A_PIN, INPUT_PULLUP);
  pinMode(FL_ENC_B_PIN, INPUT_PULLUP);
  pinMode(FR_ENC_A_PIN, INPUT_PULLUP);
  pinMode(FR_ENC_B_PIN, INPUT_PULLUP);
  pinMode(RL_ENC_A_PIN, INPUT_PULLUP);
  pinMode(RL_ENC_B_PIN, INPUT_PULLUP);
  pinMode(RR_ENC_A_PIN, INPUT_PULLUP);
  pinMode(RR_ENC_B_PIN, INPUT_PULLUP);

  // attach each A-pin to its ISR on any CHANGE
  attachInterrupt(digitalPinToInterrupt(FL_ENC_A_PIN), ISR_FL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FR_ENC_A_PIN), ISR_FR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RL_ENC_A_PIN), ISR_RL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RR_ENC_A_PIN), ISR_RR, CHANGE);
}

// simple “one-channel” quadrature decode in each ISR:
void ISR_FL() {
  bool A = digitalRead(FL_ENC_A_PIN);
  bool B = digitalRead(FL_ENC_B_PIN);
  encCount[FL] += (A == B) ? +1 : -1;
}
void ISR_FR() {
  bool A = digitalRead(FR_ENC_A_PIN);
  bool B = digitalRead(FR_ENC_B_PIN);
  encCount[FR] += (A == B) ? +1 : -1;
}
void ISR_RL() {
  bool A = digitalRead(RL_ENC_A_PIN);
  bool B = digitalRead(RL_ENC_B_PIN);
  encCount[RL] += (A == B) ? +1 : -1;
}
void ISR_RR() {
  bool A = digitalRead(RR_ENC_A_PIN);
  bool B = digitalRead(RR_ENC_B_PIN);
  encCount[RR] += (A == B) ? +1 : -1;
}

// read one wheel’s count
long readEncoder(int i) {
  if (i >= 0 && i < 4) return encCount[i];
  return 0;
}

// zero one wheel
void resetEncoder(int i) {
  if (i >= 0 && i < 4) encCount[i] = 0;
}

// zero all wheels
void resetEncoders() {
  for (int i = 0; i < 4; i++) encCount[i] = 0;
}

#else
  #error "You must define ARDUINO_ENC_COUNTER for this driver!"
#endif
