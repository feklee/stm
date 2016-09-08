#include <ArduinoJson.h>
#include <limits.h>
#include "util.hpp"

void printError(const char *message) {
  printValue("error", message);
}

template <typename T> void printValue(const char *type, T value) {
  const int bufferSize = JSON_OBJECT_SIZE(2);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &root = jsonBuffer.createObject();
  root["type"] = type;
  root["value"] = value;

  root.printTo(Serial);
  Serial.println();
}

template void printValue<unsigned long>(const char *, unsigned long);
template void printValue<long>(const char *, long);
template void printValue<float>(const char *, float);
template void printValue<uint16_t>(const char *, uint16_t);

float readVoltage(uint8_t pin) {
  return 3.3 * float(analogRead(pin)) / 0xffff;
}

// Accounts for one overflow, but not for multiple ones!
unsigned long elapsedTime(unsigned long startTime /* time units */,
                          unsigned long endTime /* time units */) {
  bool overflowHappened = endTime < startTime;
  return (overflowHappened ?
          ULONG_MAX - startTime + endTime :
          endTime - startTime);
}
