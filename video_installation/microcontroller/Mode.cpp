#include <Arduino.h>
#include "Mode.hpp"

void Mode::reset() {}

boolean Mode::step() {
  boolean continueStepping = false;
  return continueStepping;
}

const char *Mode::name() {
  return "";
}
