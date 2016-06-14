#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage,
                           Current &current, Piezo &piezo,
                           TipPositionLog &tipPositionLog) :
  motor_(motor), biasVoltage_(biasVoltage), current_(current), piezo_(piezo),
  tipPositionLog_(tipPositionLog) {}

const char *ApproachMode::name() {
  return "approach";
}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

boolean ApproachMode::displacePiezoInSteps(unsigned int increment) {
  const float targetSignal = 1; // V

  for (long i = 0; i <= 0xffff; i += increment) {
    piezo_.displace(i);
    current_.measure();
    tipPositionLog_.add(0, 0, i, current_.signal());
    if (current_.signal() >= targetSignal) {
      return true; // target signal reached
    }
    i += increment;
  }
  return false;
}

boolean ApproachMode::rotateMotor() {
  const int steps = 500;
  for (int i = 0; i < steps; i ++) {
    boolean targetSignalReached = displacePiezoInSteps(100);
    if (targetSignalReached) {
      return true;
    }
    motor_.stepDown();
  }
  return false;
}

void ApproachMode::reapproachFinely() {
  piezo_.displace(0);
  displacePiezoInSteps(10);
}

boolean ApproachMode::approach() {
  motor_.activate();
  boolean targetSignalReached = rotateMotor();
  motor_.deactivate();

  if (targetSignalReached) {
    reapproachFinely();
  }

  return targetSignalReached;
}

void ApproachMode::finish() {
  tipPositionLog_.flush();
}

boolean ApproachMode::step() {
  boolean targetSignalReached = approach();
  if (targetSignalReached) {
    return false;
  }
  finish();
  return true;
}
