#pragma once

class Fader {
  const uint8_t potPin_ = A1;
  float currentValue();
  void printJson(float);

public:
  void read();
};
