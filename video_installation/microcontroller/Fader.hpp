#pragma once

class Fader {
  int potPin_;
  float currentValue();
  void printJson(float);

public:
  Fader(int);
  void read();
};
