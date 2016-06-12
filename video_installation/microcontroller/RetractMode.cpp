#include <Arduino.h>
#include "RetractMode.hpp"

RetractMode::RetractMode(Motor &motor, BiasVoltage &biasVoltage) :
  motor_(motor), biasVoltage_(biasVoltage) {}

void RetractMode::reset() {}

boolean RetractMode::step() {
  boolean limitingSignalReached = motor_.up(500, 0.1);
  if (limitingSignalReached) {
    motor_.up(1000);
    return false;
  }
  return true;
}
