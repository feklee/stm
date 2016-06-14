// Tunneling current

#pragma once

#include <Arduino.h>

class Current {
  const uint8_t measurePin_ = A2;
  float signal_ = 0; // V

public:
  void measure();
  void print();
  float signal();
  boolean isInLimit(boolean, float);
};
