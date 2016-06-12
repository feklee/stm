#include <Arduino.h>
#include "IdleMode.hpp"

IdleMode::IdleMode() {}

void IdleMode::reset() {}

boolean IdleMode::step() {
  delay(250);
  return true;
}
