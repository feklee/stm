#include <limits.h>
#include "util.hpp"
#include "ScanMode.hpp"

ScanMode::ScanMode(TipPositionLog &tipPositionLog, Current &current,
                   Piezo &piezo) :
  tipPositionLog_(tipPositionLog), current_(current), piezo_(piezo) {
  reset();
}

const char *ScanMode::name() {
  return "scan";
}

void ScanMode::reset() {
  head_ = 0;
  startTime_ = micros();
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

bool ScanMode::headIsAtLimit() {
  const int limit = sideLen_ * sideLen_;
  return head_ == limit;
}

void ScanMode::moveTipUp() {
  for (int i = 0; i < maxAdjustmentSteps_; i ++) {
    piezo_.displaceByDelta(-adjustmentDelta_);
    current_.measure();
    bool targetSignalReached = current_.signal() < targetSignal_;
    if (targetSignalReached) {
      return;
    }
  }
}

void ScanMode::moveTipDown() {
  for (int i = 0; i < maxAdjustmentSteps_; i ++) {
    piezo_.displaceByDelta(adjustmentDelta_);
    current_.measure();
    bool targetSignalReached = current_.signal() > targetSignal_;
    if (targetSignalReached) {
      return;
    }
  }
}

void ScanMode::adjustTipHeight() {
  current_.measure();
  if (current_.signal() > targetSignal_) {
    moveTipUp();
  } else {
    moveTipDown();
  }
}

void ScanMode::scanChunk() {
  for (int j = 0; j < chunkSize_ && !headIsAtLimit(); j ++) {
    adjustTipHeight();
    uint8_t x = head_ % sideLen_;
    uint8_t y = head_ / sideLen_;
    uint16_t z = piezo_.displacement();
    tipPositionLog_.add(x, y, z, current_.signal());
    head_ ++;
  }
}

bool ScanMode::step() {
  scanChunk();
  if (headIsAtLimit()) {
    finish();
    return false;
  }
  return true;
}

void ScanMode::setSideLen(int sideLen) {
  sideLen_ = sideLen;
}

void ScanMode::setMaxAdjustmentSteps(int maxAdjustmentSteps) {
  maxAdjustmentSteps_ = maxAdjustmentSteps;
}

void ScanMode::setAdjustmentDelta(int adjustmentDelta) {
  adjustmentDelta_ = adjustmentDelta;
}
