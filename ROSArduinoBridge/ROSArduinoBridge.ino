// ROSArduinoBridge.ino

#define USE_BASE            // Enable base controller
#define ARDUINO_ENC_COUNTER // Four encoders via interrupts
#define L298_MOTOR_DRIVER   // Four PWM-only L298 channels
#undef  USE_SERVOS          // No servos in this sketch

#define BAUDRATE     57600
#define MAX_PWM      255

#include <Arduino.h>
#include "commands.h"
#include "sensors.h"

#ifdef USE_BASE
  #include "motor_driver.h"     // our 4-motor pwm driver
  #include "encoder_driver.h"         // our 4-encoder interrupt driver
  #include "diff_controller.h"    // our 4-wheel PID controller

  // PID timing
  #define PID_RATE            30     // Hz
  const int PID_INTERVAL = 1000 / PID_RATE;
  unsigned long nextPID     = PID_INTERVAL;

  // auto-stop if no commands arrive
  #define AUTO_STOP_INTERVAL 2000    // ms
  long lastMotorCommand     = AUTO_STOP_INTERVAL;
#endif

// ———————————————————————————
// Serial-command parsing state
// ———————————————————————————
char  cmd = 0;
char  argBuf[64];
int   argIdx = 0;

void resetCommand() {
  cmd    = 0;
  argIdx = 0;
  memset(argBuf, 0, sizeof(argBuf));
}

/**
 * runCommand()
 *   Parses up to four space-delimited integers in argBuf,
 *   then dispatches to the appropriate handler.
 */
int runCommand() {
  // split argBuf into tokens
  char* tokens[4] = { nullptr, nullptr, nullptr, nullptr };
  int   nTok = 0;
  char* saveptr;
  char* t = strtok_r(argBuf, " ", &saveptr);
  while (t && nTok < 4) {
    tokens[nTok++] = t;
    t = strtok_r(nullptr, " ", &saveptr);
  }

  switch (cmd) {
    // — Encoder commands —
    case READ_ENCODERS:
      for (int i = 0; i < 4; ++i) {
        Serial.print(readEncoder(i));
        if (i < 3) Serial.print(' ');
      }
      Serial.println();
      break;

    case RESET_ENCODERS:
      resetEncoders();
      resetPID();
      Serial.println("OK");
      break;

    // — Mecanum velocity via PID —
    case MOTOR_SPEEDS:
      lastMotorCommand = millis();
      {
        int speeds[4] = {0,0,0,0};
        for (int i = 0; i < nTok; ++i) speeds[i] = atoi(tokens[i]);

        bool allZero = true;
        for (int i = 0; i < 4; ++i) if (speeds[i] != 0) allZero = false;

        if (allZero) {
          setMotorSpeeds(0,0,0,0);
          resetPID();
          moving = false;
        } else {
          moving = true;
        }
        for (int i = 0; i < 4; ++i) {
          wheelPID[i].TargetTicksPerFrame = speeds[i];
        }
        Serial.println("OK");
      }
      break;

    // — Raw PWM control —
    case MOTOR_RAW_PWM:
      lastMotorCommand = millis();
      {
        int pwm[4] = {0,0,0,0};
        for (int i = 0; i < nTok; ++i) pwm[i] = atoi(tokens[i]);

        resetPID();
        moving = false;
        setMotorSpeeds(pwm[0], pwm[1], pwm[2], pwm[3]);
        Serial.println("OK");
      }
      break;

    // — Pass through the rest unchanged —
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;
    case ANALOG_READ:
      Serial.println(analogRead(atoi(tokens[0])));
      break;
    case DIGITAL_READ:
      Serial.println(digitalRead(atoi(tokens[0])));
      break;
    case ANALOG_WRITE:
      analogWrite(atoi(tokens[0]), atoi(tokens[1]));
      Serial.println("OK");
      break;
    case DIGITAL_WRITE:
      digitalWrite(atoi(tokens[0]),
                   atoi(tokens[1]) ? HIGH : LOW);
      Serial.println("OK");
      break;
    case PIN_MODE:
      pinMode(atoi(tokens[0]),
              atoi(tokens[1]) ? OUTPUT : INPUT);
      Serial.println("OK");
      break;
    case PING:
      Serial.println(Ping());
      break;

    default:
      Serial.println("Invalid Command");
      break;
  }
  return 0;
}

void setup() {
  Serial.begin(BAUDRATE);

#ifdef USE_BASE
  initEncoders();          // set up all 4 encoder interrupts
  initMotorController();   // configure all 8 PWM pins
  resetPID();              // zero all 4 PID controllers
#endif
}

void loop() {
  // — Serial input parsing —
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') {
      runCommand();
      resetCommand();
    }
    else if (c == ' ') {
      if (argIdx < sizeof(argBuf)-1) {
        argBuf[argIdx++] = '\0';  // mark end of one token
      }
    }
    else {
      if (!cmd) {
        cmd = c;                   // first non-space is the command
      } else if (argIdx < sizeof(argBuf)-1) {
        argBuf[argIdx++] = c;      // collect args
      }
    }
  }

#ifdef USE_BASE
  // — PID loop timing —
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  // — auto-stop check —
  if (millis() - lastMotorCommand > AUTO_STOP_INTERVAL) {
    setMotorSpeeds(0,0,0,0);
    moving = false;
  }
#endif
}
