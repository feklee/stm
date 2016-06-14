#include <Arduino.h>
#include "ApproachMode.hpp"

ApproachMode::ApproachMode(Motor &motor, BiasVoltage &biasVoltage,
                           Current &current, Piezo &piezo,
                           CurrentLog &currentLog) :
  motor_(motor), biasVoltage_(biasVoltage), current_(current), piezo_(piezo),
  currentLog_(currentLog) {}

void ApproachMode::reset() {
  biasVoltage_.set(50);
}

boolean ApproachMode::probeWithPiezo(unsigned int increment) {
  const float targetSignal = 1; // V

  for (long i = 0; i <= 0xffff; i += increment) {
    piezo_.displace(i);
    current_.measure();
    currentLog_.add(current_);
    if (current_.signal() >= targetSignal) {
      return true; // target signal reached
    }
    i += increment;
  }
  return false;
}

boolean ApproachMode::probe() {
  const int steps = 500;
  for (int i = 0; i < steps; i ++) {
    boolean targetSignalReached = probeWithPiezo(100);
    if (targetSignalReached) {
      return true;
    }
    motor_.stepDown();
  }
  return false;
}

void ApproachMode::reapproachFinely() {
  piezo_.displace(0);
  probeWithPiezo(10);
}

boolean ApproachMode::approach() {
  motor_.activate();
  boolean targetSignalReached = probe();
  motor_.deactivate();

  if (targetSignalReached) {
    reapproachFinely();
  }

  return targetSignalReached;
}

void ApproachMode::finish() {
  currentLog_.flush();
}

boolean ApproachMode::step() {
  boolean targetSignalReached = approach();
  if (targetSignalReached) {
    return false;
  }
  finish();
  return true;
}
