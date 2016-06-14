#pragma once

#include "CurrentSignal.hpp"

class TipPosition {
  struct Entry {
    uint8_t x;
    uint8_t y;
    uint16_t z; // piezo position
    float currentSignal; // V
  };

  static const int size_ = 100;
  Datum entries_[size_];
  int head_ = 0;
  void print();

public:
  void flush();
  void add(Entry &);
};
