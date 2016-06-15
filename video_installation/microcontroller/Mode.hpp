#pragma once

#include <Arduino.h>

class Mode {
public:
  virtual void reset();
  virtual bool step();
  virtual const char *name();
};
