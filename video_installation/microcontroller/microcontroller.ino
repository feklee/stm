// Arduino / Teensy sketch

#include "ScanData.hpp"

ScanData scanData;

void setup() {
  pinMode(A1, INPUT);
}

float readVoltageWithTeensyLC(int pin) {
  return analogRead(pin) * 3.3 / 0xffff;
}

void loop() {
  scanData.append({0, 0, 0, readVoltageWithTeensyLC(A1), micros()});
  delay(100);
}
