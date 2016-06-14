#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "Mode.hpp"

class RetractMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  boolean rotateMotor(int, float);
  boolean moveUp(int, float);

public:
  RetractMode(Motor &, BiasVoltage &, Current &);
  boolean step();
};
