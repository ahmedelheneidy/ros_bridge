#include <Arduino.h>
#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include "sensors.h"
#include "servos.h"
#include "diff_controller.h"    // <-- closed‑loop PID

void setup() {
  Serial.begin(115200);
  initMotorController();
  setupEncoders();
  resetEncoders();
  resetPID();                // initialize all PID state
  #ifdef USE_SERVOS
    for (int i = 0; i < N_SERVOS; i++)
      servos[i].initServo(servoPins[i], stepDelay[i], servoInitPosition[i]);
  #endif
  pinMode(UV_RELAY, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case DRIVE_MECANUM: {
        // read desired velocities (ticks/frame or any unit you choose)
        int vx = Serial.parseInt();    // forward/back
        int vy = Serial.parseInt();    // strafe right
        int om = Serial.parseInt();    // rotate CW

        // mecanum mixing into per‑wheel targets
        wheelPID[FL].TargetTicksPerFrame =  vx + vy + om;
        wheelPID[FR].TargetTicksPerFrame =  vx - vy - om;
        wheelPID[RL].TargetTicksPerFrame =  vx - vy + om;
        wheelPID[RR].TargetTicksPerFrame =  vx + vy - om;

        moving = true;                // enable PID loop
        Serial.println("OK");
        break;
      }

      case READ_ENCODERS: {
        long e0 = readEncoder(FL),
             e1 = readEncoder(FR),
             e2 = readEncoder(RL),
             e3 = readEncoder(RR);
        Serial.print(e0); Serial.print(',');
        Serial.print(e1); Serial.print(',');
        Serial.print(e2); Serial.print(',');
        Serial.println(e3);
        break;
      }

      case RESET_ENCODERS:
        resetEncoders();
        resetPID();                  // also reset PID integrators
        Serial.println("OK");
        break;

      case PING: {
        int pin = Serial.parseInt();
        Serial.println(Ping(pin));
        break;
      }

      case SERVO_WRITE: {
        int idx = Serial.parseInt();
        int pos = Serial.parseInt();
        servos[idx].setTargetPosition(pos);
        Serial.println("OK");
        break;
      }

      case DIGITAL_WRITE: {
        int pin = Serial.parseInt();
        int val = Serial.parseInt();
        digitalWrite(pin, val);
        Serial.println("OK");
        break;
      }

      case ANALOG_READ: {
        int pin = Serial.parseInt();
        Serial.println(analogRead(pin));
        break;
      }

      case ANALOG_WRITE: {
        int pin = Serial.parseInt();
        int val = Serial.parseInt();
        analogWrite(pin, val);
        Serial.println("OK");
        break;
      }

      case 'v':  // UV on
        digitalWrite(UV_RELAY, HIGH);
        break;

      case 'V':  // UV off
        digitalWrite(UV_RELAY, LOW);
        break;

      default:
        // unknown command—ignore
        break;
    }
  }

  // ----- CLOSED-LOOP UPDATE -----
  updatePID();                 // runs every loop, sets motor PWMs

  // Sweep servos in background
  #ifdef USE_SERVOS
    for (int i = 0; i < N_SERVOS; i++)
      servos[i].doSweep();
  #endif
}
