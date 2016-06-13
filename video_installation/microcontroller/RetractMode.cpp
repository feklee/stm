#include <Arduino.h>
#include "RetractMode.hpp"

RetractMode::RetractMode(Motor &motor, BiasVoltage &biasVoltage,
                         TipPosition &tipPosition) :
  motor_(motor), biasVoltage_(biasVoltage), tipPosition_(tipPosition) {}

boolean RetractMode::rotateMotor(int steps, float limitingSignal) {
  for (int i = 0; i < steps; i ++) {
    motor_.stepUp();
    tipPosition_.measureSignal();
    boolean limitingSignalReached =
      !tipPosition_.signalIsInLimit(false, limitingSignal); // fixme: just compare signal?
    if (limitingSignalReached) {
      return true;
    }
  }
  return false;
}

boolean RetractMode::moveUp(
  int steps,
  float limitingSignal = -1 // V, outside bounds by default
) {
  motor_.activate();
  boolean limitingSignalReached = rotateMotor(steps, limitingSignal);
  motor_.deactivate();
  return limitingSignalReached;
}

boolean RetractMode::step() {
  boolean limitingSignalReached = moveUp(500, 0.1);
  if (limitingSignalReached) {
    moveUp(1000);
    return false;
  }
  return true;
}
