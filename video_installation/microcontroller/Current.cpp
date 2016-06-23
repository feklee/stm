#include "util.hpp"
#include "Current.hpp"

void Current::measure() {
  signal_ = readVoltage(measurePin_);
  // For simulation (debugging): signal_ = 2 * float(rand()) / RAND_MAX;
}

void Current::print() {
  printValue("currentSignal", signal_);
}

float Current::signal() {
  return signal_;
}
