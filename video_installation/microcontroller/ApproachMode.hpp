#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Piezo.hpp"
#include "TipPositionLog.hpp"
#include "Mode.hpp"

class ApproachMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  Piezo &piezo_;
  TipPositionLog &tipPositionLog_;
  float targetCurrentSignal_ = 1;
  bool displacePiezoInSteps(unsigned int);
  bool rotateMotor();
  void reapproachFinely();
  bool approach();
  void finish();

public:
  ApproachMode(Motor &, BiasVoltage &, Current &, Piezo &, TipPositionLog &);
  const char *name();
  void reset();
  bool step();
  void setTargetCurrentSignal(float);
};
