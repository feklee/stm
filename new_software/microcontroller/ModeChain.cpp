#include "ModeChain.hpp"

ModeChain::ModeChain(Mode &defaultMode) : defaultMode_(defaultMode) {}

void ModeChain::clear() {
  first_ = 0;
  last_ = -1;
}

void ModeChain::append(Mode &mode) {
  bool isFull = last_ + 1 == maxSize;

  if (isFull) {
    return;
  }

  last_ ++;
  modes[last_] = &mode;
}

Mode &ModeChain::next() {
  bool isEmpty = first_ > last_;

  if (isEmpty) {
    return defaultMode_;
  }

  Mode &mode = *modes[first_];
  first_ ++;

  return mode;
}
