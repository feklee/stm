#include <Arduino.h>
#include "Motor.hpp"

void Motor::down(long steps) {
  long stepsLeft;
  activate();
  stepsLeft = rotate(steps, false, 5);
  deactivate();
  Serial.println(steps - stepsLeft); // fixme
}

void Motor::setPins(byte val0, byte val1, byte val2, byte val3) {
  digitalWrite(pins_[0], val0);
  digitalWrite(pins_[1], val1);
  digitalWrite(pins_[2], val2);
  digitalWrite(pins_[3], val3);
}

void Motor::sendPosition() {
  switch (position_) {
  case 0:
    setPins(LOW, LOW, LOW, HIGH);
    break;
  case 1:
    setPins(LOW, LOW, HIGH, HIGH);
    break;
  case 2:
    setPins(LOW, LOW, HIGH, LOW);
    break;
  case 3:
    setPins(LOW, HIGH, HIGH, LOW);
    break;
  case 4:
    setPins(LOW, HIGH, LOW, LOW);
    break;
  case 5:
    setPins(HIGH, HIGH, LOW, LOW);
    break;
  case 6:
    setPins(HIGH, LOW, LOW, LOW);
    break;
  case 7:
    setPins(HIGH, LOW, LOW, HIGH);
    break;
  }
}

void Motor::advancePosition(boolean rotateClockwise) {
  position_ += (rotateClockwise ? 1 : -1);
  position_ &= 7;
}


void Motor::step(boolean rotateClockwise) {
  sendPosition();
  advancePosition(rotateClockwise);
}

long Motor::rotate(long stepsLeft, boolean rotateClockwise,
                   float limitingSignal /* V */) {
  while (stepsLeft > 0/* fixme: && signalIsInLimit(isMovingDown(rotateClockwise),
                                          limitingSignal)*/) {
    step(rotateClockwise);
    stepsLeft --;
    delay(1);
  }

  return stepsLeft;
}

void Motor::activate() {
  digitalWrite(activationPin_, HIGH);
}

void Motor::deactivate() {
  digitalWrite(activationPin_, LOW);
}

void Motor::setup() {
  for (int i = 0; i < 4; i ++) {
    pinMode(pins_[i], OUTPUT);
  }
  pinMode(activationPin_, OUTPUT);
}
