#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Mode.hpp"

class RetractMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  TipPosition &tipPosition_; // fixme: better store Signal?
  boolean rotateMotor(int, float);
  boolean moveUp(int, float);

public:
  RetractMode(Motor &, BiasVoltage &, TipPosition &tipPosition);
  boolean step();
};
