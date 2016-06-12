#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Mode.hpp"

class ApproachMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;

public:
  ApproachMode(Motor &, BiasVoltage &);
  void reset();
  Mode *step();
};
