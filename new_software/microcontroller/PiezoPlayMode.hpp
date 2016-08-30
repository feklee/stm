#pragma once

#include "Piezo.hpp"
#include "Mode.hpp"

class PiezoPlayMode : public Mode {
  Piezo &piezo_;

public:
  PiezoPlayMode(Piezo &);
  const char *name();
  bool step();
};
