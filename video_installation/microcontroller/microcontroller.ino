#include <ArduinoJson.h>
#include "Fader.hpp"
#include "Mode.hpp"
#include "util.hpp"

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
    printError("Input too long");
    return;
  }

  strncpy(t, s.c_str(), s.length());

  JsonObject &root = jsonBuffer.parseObject(t);
  if (!root.success()) {
    printError("Parsing JSON failed");
    return;
  }

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
