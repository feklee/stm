#include <limits.h>
#include "util.hpp"
#include "ScanMode.hpp"

ScanMode::ScanMode(TipPosition &tipPosition) : tipPosition_(tipPosition) {
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
  return elapsedTime(startTime_, endTime);
}

void ScanMode::printDuration() {
  printValue("scanDuration", duration());
}

void ScanMode::finish() {
  tipPosition_.flushLog();
  printDuration();
}

boolean ScanMode::headIsAtLimit() {
  const int limit = sideLen_ * sideLen_;
  return head_ == limit;
}

void ScanMode::scanChunk() {
  for (int j = 0; j < chunkSize_ && !headIsAtLimit(); j ++) {
    tipPosition_.setX(head_ % sideLen_);
    tipPosition_.setY(head_ / sideLen_);
    tipPosition_.setZ(z_);
    tipPosition_.measureSignal();
    tipPosition_.logCurrentValues();
    advanceZ();
    head_ ++;
  }
}

boolean ScanMode::step() {
  scanChunk();
  if (headIsAtLimit()) {
    finish();
    return false;
  }
  return true;
}

void ScanMode::setSideLen(int x) {
  sideLen_ = x;
}
