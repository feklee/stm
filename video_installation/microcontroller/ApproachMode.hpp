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
  boolean displacePiezoInSteps(unsigned int);
  boolean rotateMotor();
  void reapproachFinely();
  boolean approach();
  void finish();

public:
  ApproachMode(Motor &, BiasVoltage &, Current &, Piezo &, TipPositionLog &);
  const char *name();
  void reset();
  boolean step();
};
