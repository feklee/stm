#pragma once

class Position {
  struct Datum {
    uint8_t x;
    uint8_t y;
    uint16_t z; // piezo position
    float voltage; // V
  };

  static const int chunkSize_ = 100;
  Datum log_[chunkSize_];
  Datum current_;
  int logHead_ = 0;
  void printJson();
  void flushLog();

public:
  void logCurrent();
  void setX(uint8_t);
  void setY(uint8_t);
  void setZ(uint16_t);
  void measureVoltage();
};
