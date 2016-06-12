#include <limits.h>
#include "util.hpp"
#include "ScanMode.hpp"

ScanMode::ScanMode(Position &position) : position_(position) {
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
  printValue("scanDuration", duration());
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
    position_.measureSignal();
    position_.logCurrentValues();
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
