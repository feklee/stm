// Arduino / Teensy sketch

#include <ArduinoJson.h>
#include "ScanData.hpp"
#include "Fader.hpp"

static ScanData scanData;
static Fader fader(A1);
static const int sideLen = 128;
static uint16_t z = 0xffff / 2;
enum Mode { scanning, hovering };
Mode mode = hovering;

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

void step() {
  switch (mode) {
  case scanning:
    scanStep();
    break;
  case hovering:
    break;
  }
}

void interpretSerialInput(const String &s) {
  const int maxNumberOfExpectedObjects = 20;
  const int bufferSize = JSON_OBJECT_SIZE(maxNumberOfExpectedObjects);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &root = jsonBuffer.parseObject(s);

  String requestedMode = root["mode"];
  if (requestedMode == "scanning") {
    mode = scanning;
  } else {
    mode = hovering;
  }
}

void loop() {
  if (Serial.available() > 0) {
    interpretSerialInput(Serial.readString());
  }
  step();
  fader.read();
}
