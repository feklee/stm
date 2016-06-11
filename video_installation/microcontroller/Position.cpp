// Position of the microscope.

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Position.hpp"

void Position::printJson() {
  char buffer[8];
  Serial.print("{\"type\":\"positionLog\",\"positions\":[");
  for (int i = 0; i < logHead_; i ++) {
    Datum &datum = log_[i];
    if (i > 0) {
      Serial.print(",");
    }
    Serial.print("[");
    Serial.print(datum.x);
    Serial.print(",");
    Serial.print(datum.y);
    Serial.print(",");
    Serial.print(datum.z);
    Serial.print(",");
    dtostrf(datum.voltage, 0, 2, buffer);
    Serial.print(buffer);
    Serial.print("]");
  }
  Serial.println("]}");
}

void Position::flushLog() {
  if (logHead_ > 0) {
    printJson();
    logHead_ = 0;
  }
}

void Position::logCurrent() {
  log_[logHead_] = current_;
  logHead_ ++;
  if (logHead_ >= logSize_) {
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
