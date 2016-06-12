#pragma once

class Mode {
public:
  virtual void reset() = 0;
  virtual Mode *step() = 0;
};
