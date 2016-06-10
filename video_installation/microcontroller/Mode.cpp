// Modes that the microscope can be in.

#include "Mode.hpp"

IdleMode::IdleMode(Position &position) : position_(position) {}

Mode *IdleMode::step() {
  position_.measureVoltage();
  return 0;
}

ScanMode::ScanMode(Position &position, IdleMode &successor) :
  position_(position), successor_(successor) {}

void ScanMode::advanceZ() {
  const uint16_t amplitude = 0xfff;
  z_ = max(0, min(0xffff, z_ + random(-amplitude, amplitude + 1)));
}

Mode *ScanMode::step() {
  static long i = 0;
  const int limit = sideLen_ * sideLen_;

  position_.setX(i % sideLen_);
  position_.setY(i / sideLen_);
  position_.setZ(z_);
  position_.measureVoltage();

  i ++;
  if (i == limit) {
    i = 0;
    return &successor_;
  }
  advanceZ();
  return 0;
}

void ScanMode::setSideLen(int x) {
  sideLen_ = x;
}
