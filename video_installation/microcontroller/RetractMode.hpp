#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "BiasVoltage.hpp"
#include "Current.hpp"
#include "CurrentLog.hpp"
#include "Mode.hpp"

class RetractMode : public Mode {
  Motor &motor_;
  BiasVoltage &biasVoltage_;
  Current &current_;
  CurrentLog &currentLog_;
  boolean rotateMotor(int, float);
  boolean retract(int, float);

public:
  RetractMode(Motor &, BiasVoltage &, Current &, CurrentLog &);
  boolean step();
};
