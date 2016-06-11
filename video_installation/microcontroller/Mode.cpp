// Modes that the microscope can be in.

#include <limits.h>
#include <ArduinoJson.h>
#include "Mode.hpp"

IdleMode::IdleMode() {}

void IdleMode::reset() {}

Mode *IdleMode::step() {
  return 0;
}

ScanMode::ScanMode(Position &position, IdleMode &successor) :
  position_(position), successor_(successor) {
  reset();
}

void ScanMode::reset() {
  i = 0;
  startTime = micros();
}

void ScanMode::advanceZ() {
  const uint16_t amplitude = 0xfff;
  z_ = max(0, min(0xffff, z_ + random(-amplitude, amplitude + 1)));
}

unsigned long ScanMode::duration() {
  unsigned long d = micros() - startTime;
  if (d < 0) {
    d += ULONG_MAX;
  }
  return d;
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

Mode *ScanMode::step() {
  const int limit = sideLen_ * sideLen_;

  position_.setX(i % sideLen_);
  position_.setY(i / sideLen_);
  position_.setZ(z_);
  position_.measureVoltage();
  position_.logCurrent();

  i ++;
  if (i == limit) {
    finish();
    return &successor_;
  }
  advanceZ();
  return 0;
}

void ScanMode::setSideLen(int x) {
  sideLen_ = x;
}
