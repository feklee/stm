#pragma once

#include "Current.hpp"

class CurrentLog {
  static const int size_ = 100;
  float currentSignals_[size_];
  int head_ = 0;
  void print();

public:
  void flush();
  void add(Current &);
};
