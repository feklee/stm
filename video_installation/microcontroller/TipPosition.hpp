#pragma once

#include "BiasVoltage.hpp"

class TipPosition {
  struct Datum {
    uint8_t x;
    uint8_t y;
    uint16_t z; // piezo position
    float signal; // current signal in V
  };

  static const int logSize_ = 100;
  Datum log_[logSize_];
  Datum currentValues_;
  int logHead_ = 0;
  const uint8_t signalMeasurePin_ = A2;
  void printLog();

public:
  void logCurrentValues();
  void flushLog();
  void setX(uint8_t);
  void setY(uint8_t);
  void setZ(uint16_t);
  void measureSignal();
  void setBiasVoltageFactor(float);
  boolean signalIsInLimit(boolean, float);
  void printSignal();
};
