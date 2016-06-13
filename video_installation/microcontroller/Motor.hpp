#pragma once

#include <Arduino.h>
#include "TipPosition.hpp"

class Motor {
  const uint8_t pins_[4] = {2, 3, 4, 5};
  const uint8_t activationPin_ = 9;
  byte position_ = 0;
  void setPins(byte, byte, byte, byte);
  void sendPosition();

public:
  void activate();
  void deactivate();
  void stepDown();
  void stepUp();
  void setup();
};
