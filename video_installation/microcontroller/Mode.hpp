#pragma once

#include <Arduino.h>

class Mode {
public:
  virtual void reset();
  virtual boolean step();
  virtual const char *name();
};
