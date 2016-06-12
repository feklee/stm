#include <ArduinoJson.h>
#include "util.hpp"

void printError(const char *message) {
  printValue("error", message);
}

template <typename T> void printValue(const char *type, T value) {
  const int bufferSize = JSON_OBJECT_SIZE(2);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &jsonRoot = jsonBuffer.createObject();
  jsonRoot["type"] = type;
  jsonRoot["value"] = value;

  jsonRoot.printTo(Serial);
  Serial.println();
}

template void printValue<unsigned long>(const char *, unsigned long);
template void printValue<long>(const char *, long);
template void printValue<float>(const char *, float);

float readVoltage(uint8_t pin) {
  return 3.3 * float(analogRead(pin)) / 0xffff;
}
