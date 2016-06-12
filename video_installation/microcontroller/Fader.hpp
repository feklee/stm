#pragma once

class Fader {
  const int potPin_ = A1;
  float currentValue();
  void printJson(float);

public:
  void read();
};
