#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage,
                           TipPosition &tipPosition) :
  motor_(motor), biasVoltage_(biasVoltage), tipPosition_(tipPosition) {}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

boolean ApproachMode::rotateMotor() {
  const int steps = 500;
  const float limitingSignal = 1; // V
  for (int i = 0; i < steps; i ++) {
    motor_.stepDown();
    tipPosition_.measureSignal();
    boolean limitingSignalReached =
      !tipPosition_.signalIsInLimit(true, limitingSignal); // fixme: just compare signal?
    if (limitingSignalReached) {
      return true;
    }
  }
  return false;
}

boolean ApproachMode::moveDown() {
  motor_.activate();
  boolean limitingSignalReached = rotateMotor();
  motor_.deactivate();
  return limitingSignalReached;
}

boolean ApproachMode::step() {
  boolean limitingSignalReached = moveDown();
  if (limitingSignalReached) {
    return false;
  }
  return true;
}
