#pragma once

#include <Arduino.h>
#include "TipPositionLog.hpp"
#include "Current.hpp"
#include "Mode.hpp"

class ScanMode : public Mode {
  TipPositionLog &tipPositionLog_;
  Current &current_;
  int sideLen_;
  uint16_t z_ = 0xffff / 2;
  long head_;
  unsigned long startTime_; // µs
  int chunkSize_ = 250;
  unsigned long duration();
  void printDuration();
  void advanceZ();
  void finish();
  void scanChunk();
  boolean headIsAtLimit();

public:
  ScanMode(TipPositionLog &, Current &);
  void reset();
  boolean step();
  void setSideLen(int);
};
