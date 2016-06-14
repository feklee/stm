#include <Arduino.h>
#include "RetractMode.hpp"

RetractMode::RetractMode(Motor &motor, BiasVoltage &biasVoltage,
                         Current &current) :
  motor_(motor), biasVoltage_(biasVoltage), current_(current) {}

boolean RetractMode::rotateMotor(int steps, float targetSignal) {
  for (int i = 0; i < steps; i ++) {
    motor_.stepUp();
    current_.measure();
    if (current_.signal() <= targetSignal) {
      return true;
    }
  }
  return false;
}

boolean RetractMode::moveUp(
  int steps,
  float targetSignal = -1 // V, outside bounds by default
) {
  motor_.activate();
  boolean targetSignalReached = rotateMotor(steps, targetSignal);
  motor_.deactivate();
  return targetSignalReached;
}

boolean RetractMode::step() {
  boolean targetSignalReached = moveUp(500, 0.1);
  if (targetSignalReached) {
    moveUp(1000);
    return false;
  }
  return true;
}
