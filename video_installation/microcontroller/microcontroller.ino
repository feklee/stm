#include <ArduinoJson.h>
#include "Fader.hpp"
#include "IdleMode.hpp"
#include "ScanMode.hpp"
#include "ApproachMode.hpp"
#include "util.hpp"

static Fader fader;
static Position position;
static BiasVoltage biasVoltage;
static Motor motor(position);
static IdleMode idleMode;
static ScanMode scanMode(position, idleMode);
static ApproachMode approachMode(motor, biasVoltage);
static Mode *mode = &idleMode;

void setup() {
  Serial.begin(115200);
  analogReadResolution(16);
  biasVoltage.setup();
  motor.setup();
}

void switchMode(Mode &newMode) {
  mode = &newMode;
  mode->reset();
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

  JsonObject &jsonRoot = jsonBuffer.parseObject(t);
  if (!jsonRoot.success()) {
    printError("Parsing JSON failed");
    return;
  }

  String requestedMode = jsonRoot["mode"];
  if (requestedMode == "scan") {
    scanMode.setSideLen(jsonRoot["sideLen"]);
    switchMode(scanMode);
  } else if (requestedMode == "approach") {
    switchMode(approachMode);
  } else {
    switchMode(idleMode);
  }
}

void loop() {
  Mode *successor;

  if (Serial.available() > 0) {
    interpretSerialInput(Serial.readString());
  }
  successor = mode->step();
  if (successor != 0) {
    switchMode(*successor);
  }
  fader.print();
  biasVoltage.print();
}
