#pragma once

#include "Mode.hpp"

class ModeChain {
public:
  static const int maxSize = 20;

private:
  Mode *modes[maxSize];
  int first_ = 0;
  int last_ = -1;
  Mode &defaultMode_;

public:
  ModeChain(Mode &);
  void clear();
  Mode &next();
  void append(Mode &);
};
