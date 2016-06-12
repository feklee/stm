#pragma once

#include <Arduino.h>

class BiasVoltage {
  const uint8_t chipSelectPin_ = 8;
  const uint8_t measurePin_ = A3;
  void setFactor(float);
  void printJson(float value);

public:
  void setup();
  void set(float);
  float measuredValue();
  void print();
};
