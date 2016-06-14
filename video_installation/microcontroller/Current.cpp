#include "util.hpp"
#include "Current.hpp"

void Current::measure() {
  signal_ = readVoltage(measurePin_);
}

// fixme: remove?
boolean Current::isInLimit(boolean isMovingDown, float limiting /* V */) {
  if (isMovingDown) {
    return signal_ < limiting;
  } else {
    return signal_ > limiting;
  }
}

void Current::print() {
  printValue("currentSignal", signal_);
}

float Current::signal() {
  return signal_;
}
