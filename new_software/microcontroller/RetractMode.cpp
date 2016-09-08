#include <Arduino.h>
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
  uint16_t targetSignal // 0xffff/3.3 V
) {
  for (int i = 0; i < steps; i ++) {
    motor_.stepUp();
    current_.measure();
    tipPositionLog_.add(0, 0, 0, current_.signal());
    if (current_.signal() <= targetSignal) {
      return true;
    }
  }
  return false;
}

bool RetractMode::retract(
  int steps,
  uint16_t targetSignal = 0 // 0xffff/3.3 V (fixme/todo: negative value would be better to be never reached)
) {
  piezo_.displace(0);
  motor_.activate();
  bool targetSignalReached = rotateMotor(steps, targetSignal);
  motor_.deactivate();
  return targetSignalReached;
}

void RetractMode::finish() {
  retract(250);
  tipPositionLog_.flush();
}

bool RetractMode::step() {
  bool targetSignalReached = retract(500, targetSignal_);
  if (targetSignalReached) {
    finish();
    return false;
  }
  return true;
}

void RetractMode::setTargetSignal(uint16_t targetSignal /* 0xffff/3.3 V */) {
  targetSignal_ = targetSignal;
}
