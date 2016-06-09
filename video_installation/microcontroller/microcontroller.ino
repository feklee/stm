// Arduino / Teensy sketch

#include "ScanData.hpp"
#include "Fader.hpp"

static ScanData scanData;
static Fader fader(A1);
static const int sideLen = 128;
static uint16_t z = 0xffff / 2;

void setup() {
  Serial.begin(115200);
  analogReadResolution(16);
//  pinMode(A1, INPUT);
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
  fader.read();
}
