#include <Arduino.h>
#include "IdleMode.hpp"

IdleMode::IdleMode() {}

void IdleMode::reset() {}

Mode *IdleMode::step() {
  delay(250);
  return 0;
}
