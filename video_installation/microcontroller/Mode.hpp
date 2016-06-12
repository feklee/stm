#pragma once

class Mode {
public:
  virtual void reset() = 0;
  virtual boolean step() = 0;
};
