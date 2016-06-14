#include <SPI.h>
#include <ArduinoJson.h>
#include "Fader.hpp"
#include "IdleMode.hpp"
#include "ScanMode.hpp"
#include "ApproachMode.hpp"
#include "RetractMode.hpp"
#include "PiezoPlayMode.hpp"
#include "TipPositionLog.hpp"
#include "ModeQueue.hpp"
#include "util.hpp"

static Fader fader;
static Current current;
static TipPositionLog tipPositionLog;
static BiasVoltage biasVoltage;
static Piezo piezo;
static Motor motor;
static IdleMode idleMode;
static ScanMode scanMode(tipPositionLog, current);
static ApproachMode approachMode(motor, biasVoltage, current, piezo,
                                 tipPositionLog);
static RetractMode retractMode(motor, biasVoltage, current, piezo,
                               tipPositionLog);
static PiezoPlayMode piezoPlayMode(piezo);
static Mode *mode = &idleMode;
static ModeQueue modeQueue(idleMode);

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
  const int maxQueueSize = ModeQueue::maxSize;
  const int bufferSize =
    JSON_ARRAY_SIZE(maxQueueSize) +
    maxQueueSize * maxNumberOfExpectedProperties;
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

  modeQueue.clear();
  for (unsigned int i = 0; i < root.size(); i ++) {
    modeQueue.push(modeFromNode(root[i]));
  }
  switchMode(modeQueue.pop());
}

void loop() {
  boolean continueStepping;

  if (Serial.available() > 0) {
    interpretSerialInput(Serial.readString());
  }
  continueStepping = mode->step();
  if (!continueStepping) {
    switchMode(modeQueue.pop());
  }
  fader.print();
  biasVoltage.print();
  current.measure();
  current.print();
}
