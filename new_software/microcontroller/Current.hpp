// Tunneling current

#pragma once

#include <Arduino.h>

class Current {
  const uint8_t measurePin_ = A2;
  uint16_t signal_ = 0; // 0xffff/3.3 V

public:
  void measure();
  void print();
  uint16_t signal();
};
