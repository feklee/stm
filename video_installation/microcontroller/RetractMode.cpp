#include <Arduino.h>
#include "RetractMode.hpp"

RetractMode::RetractMode(Motor &motor, BiasVoltage &biasVoltage) :
  motor_(motor), biasVoltage_(biasVoltage) {}

void RetractMode::reset() {}

Mode *RetractMode::step() {
  motor_.up(500, 0.1);
  motor_.up(500);
  return 0; // fixme: finish
}
