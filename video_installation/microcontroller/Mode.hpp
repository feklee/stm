#pragma once

#include <Arduino.h>
#include "Position.hpp"

class Mode {
public:
  virtual Mode *step() = 0;
};

class IdleMode : public Mode {
  Position &position_;

public:
  IdleMode(Position &);
  Mode *step();
};

class ScanMode : public Mode {
  Position &position_;
  int sideLen_;
  uint16_t z_ = 0xffff / 2;
  void advanceZ();
  IdleMode &successor_;

public:
  ScanMode(Position &, IdleMode &);
  Mode *step();
  void setSideLen(int);
};
