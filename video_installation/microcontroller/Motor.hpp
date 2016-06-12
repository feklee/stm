#pragma once

#include <Arduino.h>

class Motor {
  const uint8_t pins_[4] = {2, 3, 4, 5};
  const uint8_t activationPin_ = 9;
  byte position_;
  void setPins(byte, byte, byte, byte);
  void activate();
  void deactivate();
  void sendPosition();
  void advancePosition(boolean);
  void step(boolean);
  long rotate(long, boolean, float);

public:
  void down(long);
  void setup();
};
