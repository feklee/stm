#pragma once

#include <Arduino.h>
#include "Position.hpp"

class Mode {
public:
  virtual void reset() = 0;
  virtual Mode *step() = 0;
};

class IdleMode : public Mode {
  Position &position_;

public:
  IdleMode(Position &);
  void reset();
  Mode *step();
};

class ScanMode : public Mode {
  Position &position_;
  int sideLen_;
  uint16_t z_ = 0xffff / 2;
  IdleMode &successor_;
  long i;
  unsigned long startTime; // µs
  unsigned long duration();
  void printDuration();
  void advanceZ();

public:
  ScanMode(Position &, IdleMode &);
  void reset();
  Mode *step();
  void setSideLen(int);
};
