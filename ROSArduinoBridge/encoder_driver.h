#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

// Rear‑Left Encoder
#define RL_ENC_PIN_A  5
#define RL_ENC_PIN_B  4
// Rear‑Right Encoder
#define RR_ENC_PIN_A 22
#define RR_ENC_PIN_B 23
// Front‑Left Encoder
#define FL_ENC_PIN_A  3
#define FL_ENC_PIN_B  2
// Front‑Right Encoder
#define FR_ENC_PIN_A 24
#define FR_ENC_PIN_B 25

long readEncoder(int wheelIndex);
void resetEncoder(int wheelIndex);
void resetEncoders();

void setupEncoders();  // call this from setup()

#endif
