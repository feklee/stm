#include "util.hpp"
#include "Current.hpp"

void Current::measure() {
  signal_ = readVoltage(measurePin_);
}

void Current::print() {
  printValue("currentSignal", signal_);
}

float Current::signal() {
  return signal_;
}
