#pragma once

#include <Arduino.h>
#include "Motor.hpp"
#include "Mode.hpp"

class ApproachMode : public Mode {
  Motor &motor_;

public:
  ApproachMode(Motor &);
  void reset();
  Mode *step();
};
