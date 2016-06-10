#include <ArduinoJson.h>
#include "Fader.hpp"
#include "Mode.hpp"

static Fader fader(A1);
static IdleMode idleMode;
static ScanMode scanMode(&idleMode);
static Mode *mode = &idleMode;

void setup() {
  Serial.begin(115200);
  analogReadResolution(16);
}

void interpretSerialInput(const String &s) {
  const int maxNumberOfExpectedObjects = 20;
  const int bufferSize = JSON_OBJECT_SIZE(maxNumberOfExpectedObjects);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &root = jsonBuffer.parseObject(s);

  String requestedMode = root["mode"];
  if (requestedMode == "scan") {
    mode = &scanMode;
  } else {
    mode = &idleMode;
  }
}

void loop() {
  Mode *successor;

  if (Serial.available() > 0) {
    interpretSerialInput(Serial.readString());
  }
  successor = mode->step();
  if (successor != 0) {
    mode = successor;
  }
  fader.read();
}
