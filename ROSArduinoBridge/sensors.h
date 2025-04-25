#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// Ultrasonic sensor pins
static const uint8_t ULTRASONIC_TRIG_PIN = 26;
static const uint8_t ULTRASONIC_ECHO_PIN = 27;

/**
 * Convert round-trip microseconds into centimeters.
 * Speed of sound ≈ 340 m/s → 29 µs/cm, but signal travels forth and back.
 */
inline float microsecondsToCm(long microseconds) {
  return (microseconds / 29.0f) / 2.0f;
}

/**
 * Trigger the ultrasonic ping and measure echo time.
 * @returns distance in centimeters.
 */
long Ping() {
  long duration, range;
  
  // Send a clean LOW pulse
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Trigger the sensor
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  // Read the echo pulse
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

  // Convert to centimeters
  range = (long)microsecondsToCm(duration);
  return range;
}

#endif // SENSORS_H
