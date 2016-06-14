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

boolean RetractMode::rotateMotor(int steps, float targetSignal) {
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

boolean RetractMode::retract(
  int steps,
  float targetSignal = -1 // V, outside bounds by default
) {
  piezo_.displace(0);
  motor_.activate();
  boolean targetSignalReached = rotateMotor(steps, targetSignal);
  motor_.deactivate();
  return targetSignalReached;
}

boolean RetractMode::step() {
  boolean targetSignalReached = retract(500, 0.1);
  if (targetSignalReached) {
    retract(250);
    return false;
  }
  return true;
}
