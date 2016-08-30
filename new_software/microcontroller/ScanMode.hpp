#pragma once

#include <Arduino.h>
#include "TipPositionLog.hpp"
#include "Current.hpp"
#include "Piezo.hpp"
#include "Mode.hpp"

class ScanMode : public Mode {
  TipPositionLog &tipPositionLog_;
  Current &current_;
  int sideLen_;
  long head_;
  unsigned long startTime_; // µs
  int chunkSize_ = 250;
  Piezo &piezo_;
  float targetSignal_ = 1; // V
  int maxAdjustmentSteps_ = 0;
  int adjustmentDelta_ = 0;
  unsigned long duration();
  void printDuration();
  void adjustTipHeight();
  void moveTipUp();
  void moveTipDown();
  void finish();
  void scanChunk();
  bool headIsAtLimit();

public:
  ScanMode(TipPositionLog &, Current &, Piezo &);
  const char *name();
  void reset();
  bool step();
  void setSideLen(int);
  void setMaxAdjustmentSteps(int);
  void setAdjustmentDelta(int);
  void setTargetSignal(float);
};
