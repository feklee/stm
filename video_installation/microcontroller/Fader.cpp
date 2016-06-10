// Tracks position of fader from "Real" to "Ideal".

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Fader.hpp"

Fader::Fader(int potPin) : potPin_(potPin) {}

float Fader::currentValue() {
  return float(analogRead(potPin_)) / 0xffff;
}

void Fader::printJson() {
  const int bufferSize = JSON_OBJECT_SIZE(2);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &jsonRoot = jsonBuffer.createObject();
  jsonRoot["type"] = "faderUpdate";
  jsonRoot["position"] = float(round(value_ * numberOfTicks)) / numberOfTicks;

  jsonRoot.printTo(Serial);
  Serial.println();
}

boolean Fader::valuesAreClose(float x, float y) {
  float epsilon = 1. / numberOfTicks
    / 2; // Divide by two so that for example with 10 ticks, it's still
         // possible to go from 0.9 to 1 if the current value is 0.94.

  return abs(x - y) < epsilon;
}

void Fader::read() {
  float newValue = currentValue();
  if (!valuesAreClose(value_, newValue)) { // Just comparing ticked values is
                                           // not an option since it can easily
                                           // cause flicker
    value_ = newValue;
    printJson();
  }
}
