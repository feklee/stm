#pragma once

#include <Arduino.h>
#include "Position.hpp"

class Motor {
  const uint8_t pins_[4] = {2, 3, 4, 5};
  const uint8_t activationPin_ = 9;
  byte shaftPosition_ = 0;
  Position &position_;
  void setPins(byte, byte, byte, byte);
  void activate();
  void deactivate();
  void sendShaftPosition();
  void advanceShaftPosition(boolean);
  void step(boolean);
  long rotate(long, boolean, float);
  boolean isMovingDown(boolean);

public:
  Motor(Position &);
  void down(long);
  void setup();
};
