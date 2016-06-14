#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Piezo.hpp"
#include "CurrentLog.hpp"
#include "Mode.hpp"

class ApproachMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  Piezo &piezo_;
  CurrentLog &currentLog_;
  boolean probeWithPiezo(unsigned int);
  boolean probe();
  void reapproachFinely();
  boolean approach();
  void finish();

public:
  ApproachMode(Motor &, BiasVoltage &, Current &, Piezo &, CurrentLog &);
  void reset();
  boolean step();
};
