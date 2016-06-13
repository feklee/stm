// Position of the tip, i.e. x, y, z, and current signal at bias voltage

#include <Arduino.h>
#include <ArduinoJson.h>
#include "util.hpp"
#include "TipPosition.hpp"

void TipPosition::printLog() {
  char buffer[8];
  Serial.print("{\"type\":\"tipPositionLog\",\"positions\":[");
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

void TipPosition::flushLog() {
  if (logHead_ > 0) {
    printLog();
    logHead_ = 0;
  }
}

void TipPosition::logCurrentValues() {
  log_[logHead_] = currentValues_;
  logHead_ ++;
  if (logHead_ >= logSize_) {
    flushLog();
  }
}

void TipPosition::setX(uint8_t x) {
  currentValues_.x = x;
}

void TipPosition::setY(uint8_t y) {
  currentValues_.y = y;
}

void TipPosition::setZ(uint16_t z) {
  currentValues_.z = z;
}

void TipPosition::measureSignal() {
  currentValues_.signal = readVoltage(signalMeasurePin_);
}

// fixme: remove?
boolean TipPosition::signalIsInLimit(boolean isMovingDown,
                                     float limitingSignal /* V */) {
  if (isMovingDown) {
    return currentValues_.signal < limitingSignal;
  } else {
    return currentValues_.signal > limitingSignal;
  }
}

void TipPosition::printSignal() {
  printValue("signal", currentValues_.signal);
}
