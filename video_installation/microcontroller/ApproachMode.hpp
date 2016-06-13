#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Mode.hpp"

class ApproachMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  TipPosition &tipPosition_; // fixme: better store Signal?
  boolean moveDown();

public:
  ApproachMode(Motor &, BiasVoltage &, TipPosition &);
  void reset();
  boolean step();
};
