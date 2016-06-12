#include <Arduino.h>
#include <ArduinoJson.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage) :
  motor_(motor), biasVoltage_(biasVoltage) {}

void ApproachMode::reset() {
  biasVoltage_.set(10);
  motor_.down(500);
}

Mode *ApproachMode::step() {
  delay(250);
  return 0;
}
