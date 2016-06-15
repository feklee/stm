#include <Arduino.h>
#include "Mode.hpp"

void Mode::reset() {}

bool Mode::step() {
  bool continueStepping = false;
  return continueStepping;
}

const char *Mode::name() {
  return "";
}
