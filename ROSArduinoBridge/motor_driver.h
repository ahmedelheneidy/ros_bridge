#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

// Front‑Left motor pins
#define FL_IN1   13
#define FL_IN2   12
// Front‑Right motor pins
#define FR_IN1   11
#define FR_IN2   10
// Rear‑Left motor pins
#define RL_IN1    9
#define RL_IN2    8
// Rear‑Right motor pins
#define RR_IN1    7
#define RR_IN2    6

void initMotorController();
void setMotorSpeed(int wheelIndex, int spd);
void setMecanum(int vx, int vy, int omega);

#endif
