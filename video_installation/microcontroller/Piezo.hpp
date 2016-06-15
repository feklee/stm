#pragma once

#include <Arduino.h>

class Piezo {
  const uint8_t chipSelectPin_ = 7;
  uint16_t displacement_ = 0;
  void sendDisplacement();
  void addToDisplacement(uint16_t);

public:
  void setup();
  void displaceForDuration(uint16_t, unsigned long);
  void displace(uint16_t);
  void displaceByDelta(int);
  uint16_t displacement();
  void play(unsigned long);
};
