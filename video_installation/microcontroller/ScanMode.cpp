#include <limits.h>
#include <ArduinoJson.h>
#include "ScanMode.hpp"

ScanMode::ScanMode(Position &position, IdleMode &successor) :
  position_(position), successor_(successor) {
  reset();
}

void ScanMode::reset() {
  head_ = 0;
  startTime_ = micros();
}

void ScanMode::advanceZ() {
  const uint16_t amplitude = 0xfff;
  z_ = max(0, min(0xffff, z_ + random(-amplitude, amplitude + 1)));
}

unsigned long ScanMode::duration() {
  unsigned long endTime = micros();
  bool overflowHappened = endTime < startTime_;
  return (overflowHappened ?
          ULONG_MAX - startTime_ + endTime :
          endTime - startTime_);
}

void ScanMode::printDuration() {
  unsigned long d = duration();
  const int bufferSize = JSON_OBJECT_SIZE(2);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &root = jsonBuffer.createObject();
  root["type"] = "scanDuration";
  root["duration"] = d;

  root.printTo(Serial);
  Serial.println();
}

void ScanMode::finish() {
  position_.flushLog();
  printDuration();
}

boolean ScanMode::headIsAtLimit() {
  const int limit = sideLen_ * sideLen_;
  return head_ == limit;
}

void ScanMode::scanChunk() {
  for (int j = 0; j < chunkSize_ && !headIsAtLimit(); j ++) {
    position_.setX(head_ % sideLen_);
    position_.setY(head_ / sideLen_);
    position_.setZ(z_);
    position_.measureVoltage();
    position_.logCurrentValues();
    advanceZ();
    head_ ++;
  }
}

Mode *ScanMode::step() {
  scanChunk();
  if (headIsAtLimit()) {
    finish();
    return &successor_;
  }
  return 0;
}

void ScanMode::setSideLen(int x) {
  sideLen_ = x;
}
