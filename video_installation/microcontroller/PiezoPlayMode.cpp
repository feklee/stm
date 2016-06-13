#include <Arduino.h>
#include "PiezoPlayMode.hpp"

PiezoPlayMode::PiezoPlayMode(Piezo &piezo) : piezo_(piezo) {}

boolean PiezoPlayMode::step() {
  piezo_.play(250);
  return true;
}
