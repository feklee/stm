#include <Arduino.h>
#include "RetractMode.hpp"

RetractMode::RetractMode(Motor &motor, BiasVoltage &biasVoltage,
                         TipPosition &tipPosition) :
  motor_(motor), biasVoltage_(biasVoltage), tipPosition_(tipPosition) {}

boolean RetractMode::moveUp(float limitingSignal = 6 /* V, outside bounds */) {
  const int steps = 500;

  motor_.activate();
  for (int i = 0; i < steps; i ++) {
    motor_.stepUp();
    tipPosition_.measureSignal();
    boolean limitingSignalReached =
      !tipPosition_.signalIsInLimit(false, limitingSignal); // fixme: just compare signal?
    if (limitingSignalReached) {
      return true;
    }
  }
  motor_.deactivate();
  return false;
}

boolean RetractMode::step() {
  boolean limitingSignalReached = moveUp(0.1);
  if (limitingSignalReached) {
    moveUp(1000);
    return false;
  }
  return true;
}
