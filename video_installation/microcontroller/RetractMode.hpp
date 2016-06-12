#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Mode.hpp"

class RetractMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;

public:
  RetractMode(Motor &, BiasVoltage &);
  void reset();
  Mode *step();
};
