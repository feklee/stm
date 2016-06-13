#pragma once

#include "Mode.hpp"

class IdleMode : public Mode {
public:
  boolean step();
};
