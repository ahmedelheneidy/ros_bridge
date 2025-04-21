#ifndef COMMANDS_H
#define COMMANDS_H

/* Single‑letter commands sent over Serial */
#define ANALOG_READ      'a'
#define READ_ENCODERS    'e'
#define DRIVE_MECANUM    'm'   // expect: m vx vy omega
#define RESET_ENCODERS   'r'
#define PING             'p'
#define SERVO_WRITE      's'
#define DIGITAL_WRITE    'w'
#define ANALOG_WRITE     'x'
#define UV_ON            'v'
#define UV_OFF           'V'

/* Four‑wheel indices */
#define FL 0
#define FR 1
#define RL 2
#define RR 3

/* UV relay pin */
#define UV_RELAY         0

#endif // COMMANDS_H
