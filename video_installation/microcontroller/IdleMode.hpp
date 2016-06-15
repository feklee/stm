#pragma once

#include "Mode.hpp"

class IdleMode : public Mode {
public:
  const char *name();
  bool step();
};
