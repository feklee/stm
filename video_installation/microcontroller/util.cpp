#include <ArduinoJson.h>
#include "util.hpp"

void printError(const char *message) {
  const int bufferSize = JSON_OBJECT_SIZE(2);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &root = jsonBuffer.createObject();
  root["type"] = "error";
  root["message"] = message;

  root.printTo(Serial);
  Serial.println();
}

float readVoltage(uint8_t pin) {
  return float(analogRead(pin)) / 0xffff;
}
