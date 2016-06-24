#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Piezo.hpp"
#include "TipPositionLog.hpp"
#include "Mode.hpp"

class RetractMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  Piezo &piezo_;
  TipPositionLog &tipPositionLog_;
  float targetSignal_ = 0.1;
  bool rotateMotor(int, float);
  bool retract(int, float);
  void finish();

public:
  RetractMode(Motor &, BiasVoltage &, Current &, Piezo &, TipPositionLog &);
  const char *name();
  bool step();
  void setTargetSignal(float);
};
