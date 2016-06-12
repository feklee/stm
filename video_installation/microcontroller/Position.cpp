// Position of the microscope, i.e. x, y, z, and current signal at bias
// voltage

#include <Arduino.h>
#include <ArduinoJson.h>
#include "util.hpp"
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
    dtostrf(datum.signal, 0, 2, buffer);
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

void Position::logCurrentValues() {
  log_[logHead_] = currentValues_;
  logHead_ ++;
  if (logHead_ >= logSize_) {
    flushLog();
  }
}

void Position::setX(uint8_t x) {
  currentValues_.x = x;
}

void Position::setY(uint8_t y) {
  currentValues_.y = y;
}

void Position::setZ(uint16_t z) {
  currentValues_.z = z;
}

void Position::measureSignal() {
  currentValues_.signal = readVoltage(signalMeasurePin_);
}

// fixme: signal -> position?
boolean Position::signalIsInLimit(boolean isMovingDown,
                                  float limitingSignal /* V */) {
  if (isMovingDown) {
    return currentValues_.signal < limitingSignal;
  } else {
    return currentValues_.signal > limitingSignal;
  }
}
