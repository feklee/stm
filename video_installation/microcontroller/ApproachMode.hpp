#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Mode.hpp"

class ApproachMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  boolean rotateMotor();
  boolean moveDown();

public:
  ApproachMode(Motor &, BiasVoltage &, Current &);
  void reset();
  boolean step();
};
