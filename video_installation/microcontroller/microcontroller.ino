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
  const int maxStringLength = 512;
  char t[maxStringLength + 1];
  const int maxNumberOfExpectedObjects = 20;
  const int bufferSize = JSON_OBJECT_SIZE(maxNumberOfExpectedObjects);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  if (s.length() > maxStringLength) {
    return; // silent failure
  }

  strncpy(t, s.c_str(), s.length());

  JsonObject &root = jsonBuffer.parseObject(t); // fixme: check for error

  String requestedMode = root["mode"];
  if (requestedMode == "scan") {
    scanMode.setSideLen(root["sideLen"]);
    // fixme: flush!
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
