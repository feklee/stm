#pragma once

#include <Arduino.h>
#include "TipPosition.hpp"

class Motor {
  const uint8_t pins_[4] = {2, 3, 4, 5};
  const uint8_t activationPin_ = 9;
  byte position_ = 0;
  TipPosition &tipPosition_;
  void setPins(byte, byte, byte, byte);
  void activate();
  void deactivate();
  void sendPosition();
  void advancePosition(boolean);
  void step(boolean);
  long rotate(long, boolean, float);
  boolean isMovingDown(boolean);

public:
  Motor(TipPosition &);
  boolean down(long, float = 6);
  boolean up(long, float = -1);
  void setup();
};
