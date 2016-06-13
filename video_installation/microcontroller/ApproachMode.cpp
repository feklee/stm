#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage,
                           TipPosition &tipPosition) :
  motor_(motor), biasVoltage_(biasVoltage), tipPosition_(tipPosition) {}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

boolean ApproachMode::moveDown() {
  const int steps = 500;
  const float limitingSignal = 1; // V

  motor_.activate();
  for (int i = 0; i < steps; i ++) {
    motor_.stepDown();
    tipPosition_.measureSignal();
    boolean limitingSignalReached =
      !tipPosition_.signalIsInLimit(true, limitingSignal); // fixme: just compare signal?
    if (limitingSignalReached) {
      return true;
    }
  }
  motor_.deactivate();
  return false;
}

boolean ApproachMode::step() {
  boolean limitingSignalReached = moveDown();
  if (limitingSignalReached) {
    return false;
  }
  return true;
}
