#include <Arduino.h>
#include "util.hpp"
#include "RetractMode.hpp"

RetractMode::RetractMode(Motor &motor, BiasVoltage &biasVoltage,
                         Current &current, Piezo &piezo,
                         TipPositionLog &tipPositionLog) :
  motor_(motor), biasVoltage_(biasVoltage), current_(current),
  piezo_(piezo), tipPositionLog_(tipPositionLog) {}

const char *RetractMode::name() {
  return "retract";
}

bool RetractMode::rotateMotor(
  int steps,
  uint16_t targetSignal = 0, // 0xffff/3.3 V
  bool stopAtTarget = false
) {
  for (int i = 0; i < steps; i ++) {
    motor_.stepUp();
    current_.measure();
    tipPositionLog_.add(0, 0, 0, current_.signal());
    if (stopAtTarget && current_.signal() <= targetSignal) {
      return true;
    }
  }
  return false;
}

void RetractMode::retract(
  int steps
) {
  motor_.activate();
  rotateMotor(steps);
  motor_.deactivate();
}

bool RetractMode::retractToTarget(
  int steps,
  uint16_t targetSignal // 0xffff/3.3 V
) {
  motor_.activate();
  bool targetSignalReached = rotateMotor(steps, targetSignal, true);
  motor_.deactivate();
  return targetSignalReached;
}

void RetractMode::finish() {
  retract(250);
  tipPositionLog_.flush();
}

bool RetractMode::step() {
  piezo_.displace(0);
  bool targetSignalReached = retractToTarget(500, targetSignal_);
  if (targetSignalReached) {
    finish();
    return false;
  }
  return true;
}

void RetractMode::setTargetSignal(
  uint16_t targetSignal // 0xffff/3.3 V
) {
  targetSignal_ = targetSignal;
  printValue("targetSignal", voltFromInteger(targetSignal_));
}
