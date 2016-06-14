#include "ModeQueue.hpp"

ModeQueue::ModeQueue(Mode &defaultMode) : defaultMode_(defaultMode) {}

void ModeQueue::clear() {
  front_ = size_ = 0;
}

int ModeQueue::end() {
  return (front_ + size_) % maxSize;
}

void ModeQueue::push(Mode &mode) {
  modes[end()] = &mode;
  size_ = (size_ + 1) % maxSize;
}

Mode &ModeQueue::pop() {
  if (size_ == 0) {
    return defaultMode_;
  }

  Mode &mode = *modes[front_];
  front_ = (front_ + 1) % maxSize;
  size_ --;
  return mode;
}
