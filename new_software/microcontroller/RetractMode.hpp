#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Piezo.hpp"
#include "TipPositionLog.hpp"
#include "Mode.hpp"
#include "util.hpp"

class RetractMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  Piezo &piezo_;
  TipPositionLog &tipPositionLog_;
  uint16_t targetSignal_ = integerFromVolt(0.1); // 0xffff/3.3 V
  bool rotateMotor(int, uint16_t, bool);
  void retract(int);
  bool retractToTarget(int, uint16_t);
  void finish();

public:
  RetractMode(Motor &, BiasVoltage &, Current &, Piezo &, TipPositionLog &);
  const char *name();
  bool step();
  void setTargetSignal(float);
};
