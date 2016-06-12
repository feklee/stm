#include <Arduino.h>
#include <ArduinoJson.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage) :
  motor_(motor), biasVoltage_(biasVoltage) {}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

Mode *ApproachMode::step() {
  motor_.down(500, 1);
  return 0;
}
