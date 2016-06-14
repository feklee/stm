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
  unsigned long duration();
  void printDuration();
  void advanceZ();
  void finish();
  void scanChunk();
  boolean headIsAtLimit();

public:
  ScanMode(TipPositionLog &, Current &, Piezo &);
  const char *name();
  void reset();
  boolean step();
  void setSideLen(int);
};
