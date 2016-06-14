#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage,
                           Current &current) :
  motor_(motor), biasVoltage_(biasVoltage), current_(current) {}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

boolean ApproachMode::rotateMotor() {
  const int steps = 500;
  const float targetSignal = 1; // V
  for (int i = 0; i < steps; i ++) {
    motor_.stepDown();
    current_.measure();
    if (current_.signal() >= targetSignal) {
      return true;
    }
  }
  return false;
}

boolean ApproachMode::moveDown() {
  motor_.activate();
  boolean targetSignalReached = rotateMotor();
  motor_.deactivate();
  return targetSignalReached;
}

boolean ApproachMode::step() {
  boolean targetSignalReached = moveDown();
  if (targetSignalReached) {
    return false;
  }
  return true;
}
