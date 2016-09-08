#pragma once

class TipPositionLog {
  struct Entry {
    uint8_t x;
    uint8_t y;
    uint16_t z; // piezo position
    uint16_t currentSignal; // 0xffff/3.3 V
  };

  static const int size_ = 100;
  Entry entries_[size_];
  int head_ = 0;
  void print();

public:
  void flush();
  void add(uint8_t, uint8_t, uint16_t, uint16_t);
};
