#pragma once

struct ScanDatum {
  uint8_t x;
  uint8_t y;
  uint16_t z; // piezo position
  float voltage; // V
  unsigned long timestamp; // µs since program started (wraps)
};

class ScanData {
  static const int chunkSize_ = 32;
  ScanDatum data_[chunkSize_];
  int head_ = 0;
  void printJson();
  void flushScanData();

public:
  void append(ScanDatum);
};
