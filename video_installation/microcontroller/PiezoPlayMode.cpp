#include <Arduino.h>
#include "PiezoPlayMode.hpp"

PiezoPlayMode::PiezoPlayMode(Piezo &piezo) : piezo_(piezo) {}

const char *PiezoPlayMode::name() {
  return "piezoPlay";
}

boolean PiezoPlayMode::step() {
  piezo_.play(250);
  return true;
}
