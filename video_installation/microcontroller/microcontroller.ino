#include <SPI.h>
#include <ArduinoJson.h>
#include "Fader.hpp"
#include "IdleMode.hpp"
#include "ScanMode.hpp"
#include "ApproachMode.hpp"
#include "RetractMode.hpp"
#include "PiezoPlayMode.hpp"
#include "TipPositionLog.hpp"
#include "ModeChain.hpp"
#include "util.hpp"

static Fader fader;
static Current current;
static TipPositionLog tipPositionLog;
static BiasVoltage biasVoltage;
static Piezo piezo;
static Motor motor;
static IdleMode idleMode;
static ScanMode scanMode(tipPositionLog, current, piezo);
static ApproachMode approachMode(motor, biasVoltage, current, piezo,
                                 tipPositionLog);
static RetractMode retractMode(motor, biasVoltage, current, piezo,
                               tipPositionLog);
static PiezoPlayMode piezoPlayMode(piezo);
static Mode *mode = &idleMode;
static ModeChain modeChain(idleMode);

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
  printValue("newMode", mode->name());
}

Mode &modeFromNode(JsonObject &node) {
  String name = node["mode"].asString();
  if (name == scanMode.name()) {
    scanMode.setSideLen(node["sideLen"]);
    scanMode.setMaxAdjustmentSteps(node["maxAdjustmentSteps"]);
    scanMode.setAdjustmentDelta(node["adjustmentDelta"]);
    return scanMode;
  } else if (name == approachMode.name()) {
    return approachMode;
  } else if (name == retractMode.name()) {
    return retractMode;
  } else if (name == piezoPlayMode.name()) {
    return piezoPlayMode;
  } else {
    return idleMode;
  }
}

void interpretSerialInput(const String &s) {
  const int maxStringLength = 512;
  char t[maxStringLength + 1];
  const int maxNumberOfExpectedProperties = 20;
  const int maxChainSize = ModeChain::maxSize;
  const int bufferSize =
    JSON_ARRAY_SIZE(maxChainSize) +
    maxChainSize * maxNumberOfExpectedProperties;
  StaticJsonBuffer<bufferSize> jsonBuffer;

  if (s.length() > maxStringLength) {
    printError("Input too long");
    return;
  }

  strncpy(t, s.c_str(), s.length());

  JsonArray &root = jsonBuffer.parseArray(t);
  if (!root.success()) {
    printError("Parsing JSON failed");
    return;
  }

  modeChain.clear();
  for (unsigned int i = 0; i < root.size(); i ++) {
    modeChain.append(modeFromNode(root[i]));
  }
  switchMode(modeChain.next());
}

void loop() {
  bool continueStepping;

  if (Serial.available() > 0) {
    interpretSerialInput(Serial.readString());
  }
  continueStepping = mode->step();
  if (!continueStepping) {
    switchMode(modeChain.next());
  }
  fader.print();
  biasVoltage.print();
  current.measure();
  current.print();
}
