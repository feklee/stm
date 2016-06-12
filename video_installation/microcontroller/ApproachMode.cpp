#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor) : motor_(motor) {}

void ApproachMode::reset() {
  motor_.down(4000);
}

Mode *ApproachMode::step() {
  delay(250);
  return 0;
}
