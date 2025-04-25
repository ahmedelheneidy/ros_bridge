#ifndef COMMANDS_H
#define COMMANDS_H

// --- Serial command letters ---
#define ANALOG_READ    'a'
#define GET_BAUDRATE   'b'
#define PIN_MODE       'c'
#define DIGITAL_READ   'd'
#define READ_ENCODERS  'e'
#define MOTOR_SPEEDS   'm'   // now takes 4 ints: FL FR RL RR
#define MOTOR_RAW_PWM  'o'   // now takes 4 ints: FL FR RL RR
#define PING           'p'
#define RESET_ENCODERS 'r'
#define SERVO_WRITE    's'
#define SERVO_READ     't'
#define UPDATE_PID     'u'
#define DIGITAL_WRITE  'w'
#define ANALOG_WRITE   'x'

// --- Wheel indices for mecanum drive ---
#define FL 0   // Front-Left
#define FR 1   // Front-Right
#define RL 2   // Rear-Left
#define RR 3   // Rear-Right

#endif // COMMANDS_H
