#pragma once

#include "Mode.hpp"

class ModeQueue {
public:
  static const int maxSize = 20;

private:
  Mode *modes[maxSize];
  int front_ = 0;
  int size_ = 0;
  Mode &defaultMode_;
  int end();

public:
  ModeQueue(Mode &);
  void clear();
  Mode &pop();
  void push(Mode &);
};
