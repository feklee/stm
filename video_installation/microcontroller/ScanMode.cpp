#include <limits.h>
#include "util.hpp"
#include "ScanMode.hpp"

ScanMode::ScanMode(TipPositionLog &tipPositionLog, Current &current) :
  tipPositionLog_(tipPositionLog), current_(current) {
  reset();
}

const char *ScanMode::name() {
  return "scan";
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
  tipPositionLog_.flush();
  printDuration();
}

boolean ScanMode::headIsAtLimit() {
  const int limit = sideLen_ * sideLen_;
  return head_ == limit;
}

void ScanMode::scanChunk() {
  for (int j = 0; j < chunkSize_ && !headIsAtLimit(); j ++) {
    uint8_t x = head_ % sideLen_;
    uint8_t y = head_ / sideLen_;
    current_.measure();
    tipPositionLog_.add(x, y, z_, current_.signal());
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
