#include "util.hpp"
#include "Current.hpp"

void Current::measure() {
  signal_ = readVoltage(measurePin_);
#if 1
  signal_ = 2 * float(rand()) / RAND_MAX; // For simulation (debugging)
#endif
}

void Current::print() {
  printValue("currentSignal", signal_);
}

float Current::signal() {
  return signal_;
}
