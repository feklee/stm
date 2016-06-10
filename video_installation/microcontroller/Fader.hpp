#pragma once

class Fader {
  float value_ = -1;
  int potPin_;
  const int numberOfTicks = 20; // more ticks = higher resolution but more
                                // possible flickering

  float currentValue();
  void printJson();
  boolean valuesAreClose(float, float);

public:
  Fader(int);
  void read();
};
