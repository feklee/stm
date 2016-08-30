#pragma once

#include <Arduino.h>

class Motor {
  const uint8_t pins_[4] = {5, 4, 3, 2};
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
