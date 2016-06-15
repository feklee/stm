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

bool ApproachMode::displacePiezoInSteps(uint16_t stepSize) {
  for (long i = 0; i <= 0xffff; i += stepSize) {
    piezo_.displace(i);
    current_.measure();
    tipPositionLog_.add(0, 0, i, current_.signal());
    if (current_.signal() >= targetCurrentSignal_) {
      return true; // target signal reached
    }
  }
  return false;
}

bool ApproachMode::rotateMotor() {
  const int steps = 500;
  for (int i = 0; i < steps; i ++) {
    bool targetSignalReached = displacePiezoInSteps(coarsePiezoStepSize_);
    if (targetSignalReached) {
      return true;
    }
    motor_.stepDown();
  }
  return false;
}

void ApproachMode::reapproachFinely() {
  piezo_.displace(0);
  displacePiezoInSteps(finePiezoStepSize_);
}

bool ApproachMode::approach() {
  motor_.activate();
  bool targetSignalReached = rotateMotor();
  motor_.deactivate();

  if (targetSignalReached) {
    reapproachFinely();
  }

  return targetSignalReached;
}

void ApproachMode::finish() {
  tipPositionLog_.flush();
}

bool ApproachMode::step() {
  bool targetSignalReached = approach();
  if (targetSignalReached) {
    return false;
  }
  finish();
  return true;
}

void ApproachMode::setTargetCurrentSignal(float targetCurrentSignal) {
  targetCurrentSignal_ = targetCurrentSignal;
}

void ApproachMode::setFinePiezoStepSize(uint16_t finePiezoStepSize) {
  finePiezoStepSize_ = finePiezoStepSize;
}

void ApproachMode::setCoarsePiezoStepSize(uint16_t coarsePiezoStepSize) {
  coarsePiezoStepSize_ = coarsePiezoStepSize;
}
