#pragma once

#include <Arduino.h>

class Piezo {
  const uint8_t chipSelectPin_ = 7;
  long displacement_ = 0;
  void displace();

public:
  void setup();
  void displaceForDuration(long, unsigned long);
  void play(unsigned long);
};
