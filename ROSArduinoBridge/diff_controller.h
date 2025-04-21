#ifndef DIFF_CONTROLLER_H
#define DIFF_CONTROLLER_H

#include "encoder_driver.h"
#include "motor_driver.h"
#include "commands.h"

// Maximum PWM value for motor outputs
#define MAX_PWM 255

/* PID setpoint structure for each wheel */
typedef struct {
  double TargetTicksPerFrame;    // Desired encoder ticks per update interval
  long   Encoder;                // Current encoder count
  long   PrevEnc;                // Encoder count at last update
  int    PrevInput;              // Last measured tick delta
  int    ITerm;                  // Integrated term for I component
  long   output;                 // Last output PWM value
} SetPointInfo;

// PID parameters (tune these as needed)
static int Kp = 20;
static int Ki = 0;
static int Kd = 12;
static int Ko = 50;

// Array of PID controllers, one per wheel (FL, FR, RL, RR)
static SetPointInfo wheelPID[4];

// Flag indicating whether the robot should be regulating speed
static bool moving = false;

/**
 * Initialize all PID controllers to the current encoder values.
 */
void resetPID() {
  for (int i = 0; i < 4; i++) {
    wheelPID[i].TargetTicksPerFrame = 0.0;
    wheelPID[i].Encoder             = readEncoder(i);
    wheelPID[i].PrevEnc             = wheelPID[i].Encoder;
    wheelPID[i].output              = 0;
    wheelPID[i].PrevInput           = 0;
    wheelPID[i].ITerm               = 0;
  }
}

/**
 * Compute PID output for a single wheel.
 */
void doPID(SetPointInfo *p) {
  long Perror;
  int  input;
  long newOutput;

  // Calculate measurement and error
  input  = p->Encoder - p->PrevEnc;
  Perror = p->TargetTicksPerFrame - input;

  // PID formula: P + D + I, avoiding derivative kick
  newOutput  = (Kp * Perror - Kd * (input - p->PrevInput) + p->ITerm) / Ko;
  p->PrevEnc = p->Encoder;

  // Accumulate on top of the previous output
  newOutput += p->output;

  // Constrain to PWM limits and update integral term if not saturated
  if (newOutput >= MAX_PWM) {
    newOutput = MAX_PWM;
  } else if (newOutput <= -MAX_PWM) {
    newOutput = -MAX_PWM;
  } else {
    p->ITerm += Ki * Perror;
  }

  p->output    = newOutput;
  p->PrevInput = input;
}

/**
 * Update all PID controllers and drive motors.
 * Call this regularly (e.g. in loop()).
 */
void updatePID() {
  // Read all encoders
  for (int i = 0; i < 4; i++) {
    wheelPID[i].Encoder = readEncoder(i);
  }

  // If not moving, reset if needed and skip
  if (!moving) {
    for (int i = 0; i < 4; i++) {
      if (wheelPID[i].PrevInput != 0) {
        resetPID();
        break;
      }
    }
    return;
  }

  // Compute and apply PID for each wheel
  for (int i = 0; i < 4; i++) {
    doPID(&wheelPID[i]);
    setMotorSpeed(i, wheelPID[i].output);
  }
}

/**
 * Set target speeds (ticks/frame) for each wheel and enable PID.
 * Call before updatePID(). Example: wheelPID[FL].TargetTicksPerFrame = desiredFL;
 */

#endif // DIFF_CONTROLLER_H
