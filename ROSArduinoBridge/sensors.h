#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

float microsecondsToCm(long microseconds) {
  return microseconds / 29.0 / 2.0;
}

long Ping(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH);
  return (long)microsecondsToCm(duration);
}

#endif
