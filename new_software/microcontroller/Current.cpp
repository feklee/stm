#include "util.hpp"
#include "Current.hpp"

void Current::measure() {
  signal_ = analogRead(measurePin_);
#if 0
  signal_ = long(0xffff * float(rand()) / RAND_MAX); // For simulation
                                                     // (debugging)
#endif
}

void Current::print() {
  printValue("currentSignal", signal_);
}

uint16_t Current::signal() { // 0xffff/3.3 V
  return signal_;
}
