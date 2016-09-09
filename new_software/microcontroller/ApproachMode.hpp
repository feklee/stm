#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Piezo.hpp"
#include "TipPositionLog.hpp"
#include "Mode.hpp"
#include "util.hpp"

class ApproachMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  Piezo &piezo_;
  TipPositionLog &tipPositionLog_;
  uint16_t targetSignal_ = integerFromVolt(1); // 0xffff/3.3 V
  uint16_t finePiezoStepSize_ = 10;
  uint16_t coarsePiezoStepSize_ = 100;
  float lastSignal_;
  bool displacePiezoInSteps(uint16_t);
  bool rotateMotor();
  void reapproachFinely();
  bool approach();
  void finish();

public:
  ApproachMode(Motor &, BiasVoltage &, Current &, Piezo &, TipPositionLog &);
  const char *name();
  void reset();
  bool step();
  void setTargetSignal(float);
  void setFinePiezoStepSize(uint16_t);
  void setCoarsePiezoStepSize(uint16_t);
};
