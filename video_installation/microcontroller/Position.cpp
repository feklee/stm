// Position of the microscope in space-time.

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Position.hpp"

void Position::printJson() {
  const int bufferSize = JSON_OBJECT_SIZE(2) +
    JSON_ARRAY_SIZE(chunkSize_) + chunkSize_ * JSON_ARRAY_SIZE(4);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &jsonRoot = jsonBuffer.createObject();
  jsonRoot["type"] = "positionLog";

  JsonArray &jsonPositions = jsonRoot.createNestedArray("positions");

  for (int i = 0; i < logHead_; i ++) {
    Datum &datum = log_[i];
    JsonArray &jsonDatum = jsonPositions.createNestedArray();
    jsonDatum.add(datum.x);
    jsonDatum.add(datum.y);
    jsonDatum.add(datum.z);
    jsonDatum.add(datum.voltage);
  }

  jsonRoot.printTo(Serial);
  Serial.println();
}

void Position::flushLog() {
  printJson();
  logHead_ = 0;
}

void Position::logCurrent() {
  log_[logHead_] = current_;
  logHead_ ++;
  if (logHead_ >= chunkSize_) {
    flushLog();
  }
}

void Position::setX(uint8_t x) {
  current_.x = x;
}

void Position::setY(uint8_t y) {
  current_.y = y;
}

void Position::setZ(uint16_t z) {
  current_.z = z;
}

void Position::measureVoltage() {
  current_.voltage = 0.5;
}
