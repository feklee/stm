// Tracks position of fader from "Real" to "Ideal".

#include <Arduino.h>
#include <ArduinoJson.h>
#include "util.hpp"
#include "Fader.hpp"

void Fader::printJson(float value) {
  const int bufferSize = JSON_OBJECT_SIZE(2);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &jsonRoot = jsonBuffer.createObject();
  jsonRoot["type"] = "faderUpdate";
  jsonRoot["position"] = value;

  jsonRoot.printTo(Serial);
  Serial.println();
}

void Fader::read() {
  printJson(readVoltage(potPin_));
}
