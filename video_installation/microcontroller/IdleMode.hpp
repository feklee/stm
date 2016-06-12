#pragma once

#include "Mode.hpp"

class IdleMode : public Mode {
public:
  IdleMode();
  void reset();
  boolean step();
};
