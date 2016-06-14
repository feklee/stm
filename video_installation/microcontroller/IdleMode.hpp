#pragma once

#include "Mode.hpp"

class IdleMode : public Mode {
public:
  const char *name();
  boolean step();
};
