#include "util.hpp"
#include "Current.hpp"

void Current::measure() {
  signal_ = readVoltage(measurePin_);
  signal_ = 2 * float(rand()) / RAND_MAX; // fixme: remove
}

void Current::print() {
  printValue("currentSignal", signal_);
}

float Current::signal() {
  return signal_;
}
