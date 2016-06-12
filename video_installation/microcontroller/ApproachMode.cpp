#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage) :
  motor_(motor), biasVoltage_(biasVoltage) {}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

boolean ApproachMode::step() {
  boolean limitingSignalReached = motor_.down(500, 1);
  if (limitingSignalReached) {
    return false;
  }
  return true;
}
