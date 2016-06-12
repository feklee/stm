// Tracks position of fader from "Real" to "Ideal".

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Fader.hpp"

Fader::Fader(int potPin) : potPin_(potPin) {}

float Fader::currentValue() {
  return float(analogRead(potPin_)) / 0xffff;
}

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
  printJson(currentValue());
}
