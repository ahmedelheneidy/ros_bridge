// mecanum_drive.h
#ifndef MECANUM_DRIVE_H
#define MECANUM_DRIVE_H

#include "encoders.h"
#include "motor_driver.h"

// number of wheels
#define NUM_WHEELS 4

// wheel indices — must match motor_driver.h
#define FL 0
#define FR 1
#define RL 2
#define RR 3

// PID setpoint/state for one wheel
typedef struct {
  double TargetTicksPerFrame;  // desired encoder‐ticks/frame
  long   Encoder;              // most recent encoder reading
  long   PrevEnc;              // prior encoder reading
  int    PrevInput;            // last delta‐ticks
  int    ITerm;                // integral term
  long   output;               // last output PWM
} SetPointInfo;

// one PID struct per wheel
static SetPointInfo wheelPID[NUM_WHEELS];

// PID gains (tune as needed)
static int Kp = 20;
static int Kd = 12;
static int Ki = 0;
static int Ko = 50;  // scale factor

// flag: are we currently driving?
static bool moving = false;

/**
 * Zero and initialize all four PIDs.
 */
void resetPID(){
  for(int i = 0; i < NUM_WHEELS; i++){
    wheelPID[i].TargetTicksPerFrame = 0.0;
    wheelPID[i].Encoder  = readEncoder(i);
    wheelPID[i].PrevEnc  = wheelPID[i].Encoder;
    wheelPID[i].output   = 0;
    wheelPID[i].PrevInput= 0;
    wheelPID[i].ITerm    = 0;
  }
}

/**
 * Run the PID update on one wheel.
 */
void doPID(SetPointInfo *p) {
  // how many ticks since last frame
  int input = p->Encoder - p->PrevEnc;
  long err  = (long)p->TargetTicksPerFrame - input;

  // PID calculation (avoiding derivative kick)
  long out = (Kp * err - Kd * (input - p->PrevInput) + p->ITerm) / Ko;
  p->PrevEnc = p->Encoder;
  out += p->output;

  // saturate & accumulate integral if not saturated
  if (out >  MAX_PWM)       out =  MAX_PWM;
  else if (out < -MAX_PWM)  out = -MAX_PWM;
  else                       p->ITerm += Ki * err;

  p->output    = out;
  p->PrevInput = input;
}

/**
 * Read all encoders, run PID on each, then set motor speeds.
 * Call this at your control‐loop rate.
 */
void updatePID() {
  // 1) sample all encoders
  for(int i = 0; i < NUM_WHEELS; i++){
    wheelPID[i].Encoder = readEncoder(i);
  }

  // 2) if we're stopped, reset once
  if (!moving) {
    bool needReset = false;
    for(int i = 0; i < NUM_WHEELS; i++){
      if (wheelPID[i].PrevInput != 0) {
        needReset = true;
        break;
      }
    }
    if (needReset) resetPID();
    return;
  }

  // 3) run PID for each wheel
  for(int i = 0; i < NUM_WHEELS; i++){
    doPID(&wheelPID[i]);
  }

  // 4) send PWM commands to all four motors
  setMotorSpeeds(
    wheelPID[FL].output,
    wheelPID[FR].output,
    wheelPID[RL].output,
    wheelPID[RR].output
  );
}

#endif // MECANUM_DRIVE_H
