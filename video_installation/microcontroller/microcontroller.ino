#include <SPI.h>
#include <ArduinoJson.h>
#include "Fader.hpp"
#include "IdleMode.hpp"
#include "ScanMode.hpp"
#include "ApproachMode.hpp"
#include "RetractMode.hpp"
#include "PiezoPlayMode.hpp"
#include "util.hpp"

static Fader fader;
static TipPosition tipPosition;
static BiasVoltage biasVoltage;
static Piezo piezo;
static Motor motor;
static IdleMode idleMode;
static ScanMode scanMode(tipPosition);
static ApproachMode approachMode(motor, biasVoltage, tipPosition);
static RetractMode retractMode(motor, biasVoltage, tipPosition);
static PiezoPlayMode piezoPlayMode(piezo);
static Mode *mode = &idleMode;

void setup() {
  Serial.begin(115200);
  analogReadResolution(16);

  biasVoltage.setup();
  piezo.setup();
  motor.setup();

  SPI.begin();
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
  } else if (requestedMode == "retract") {
    switchMode(retractMode);
  } else if (requestedMode == "piezoPlay") {
    switchMode(piezoPlayMode);
  } else {
    switchMode(idleMode);
  }
}

void loop() {
  boolean continueStepping;

  if (Serial.available() > 0) {
    interpretSerialInput(Serial.readString());
  }
  continueStepping = mode->step();
  if (!continueStepping) {
    switchMode(idleMode);
  }
  fader.print();
  biasVoltage.print();
  tipPosition.measureSignal();
  tipPosition.printSignal();
}
