// Arduino / Teensy sketch

#include "ScanData.hpp"

static ScanData scanData;
static const int sideLen = 128;
static uint16_t z = 0xffff / 2;

void setup() {
//  pinMode(A1, INPUT);
}

float readVoltageWithTeensyLC(int pin) {
  return analogRead(pin) * 3.3 / 0xffff;
}

void updateZ() {
  const uint16_t amplitude = 0xfff;
  z = max(0, min(0xffff, z + random(-amplitude, amplitude + 1)));
}

void scanStep() {
  static long i = 0;
  ScanDatum d;
  d.x = i % sideLen;
  d.y = i / sideLen;
  d.z = z;
  d.voltage = 0.5;
  d.timestamp = micros();
  scanData.append(d);
  i ++;
  updateZ();
  i %= sideLen * sideLen;
}

void loop() {
  scanStep();
//  delay(10);
}
