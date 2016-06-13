#include <Arduino.h>
#include "IdleMode.hpp"

boolean IdleMode::step() {
  delay(250);
  return true;
}
