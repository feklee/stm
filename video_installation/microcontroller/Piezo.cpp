#include <SPI.h> // hard coded pins: 11 = MOSI, 13 = SCK
#include "util.hpp"
#include "Piezo.hpp"

void Piezo::setup() {
  pinMode(chipSelectPin_, OUTPUT);
  digitalWrite(chipSelectPin_, HIGH);
}

void Piezo::displace() {
  byte dacChannel = 0;
  digitalWrite(chipSelectPin_, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  SPI.transfer(dacChannel);
  SPI.transfer16(displacement_);
  SPI.endTransaction();
  digitalWrite(chipSelectPin_, HIGH);
}

void Piezo::displaceForDuration(long displacement,
                                unsigned long duration /* ms */) {
  unsigned long startTime = millis();
  displacement_ = displacement;
  displace();
  unsigned long endTime = millis();
  unsigned long e = elapsedTime(startTime, endTime);
  if (e >= duration) {
    return;
  }
  unsigned long waitTime = duration - e;
  delay(waitTime);
}

void Piezo::play(unsigned long duration /* ms */) {
  float frequency = 440; // Hz
  unsigned long startTime = millis();

  while (elapsedTime(startTime, millis()) < duration) {
    long d = 1000 / frequency / 2; // ms
    displaceForDuration(0xffff, d);
    displaceForDuration(0, d);
  }
}

#if 0 // fixme

long Piezo::singleStep(long stepsLeft, boolean moveDown,
                       boolean limitSignal,
                       float limitingSignal /* V */,
                       long stepIncrement) {
  int direction = moveDown ? 1 : -1;

  while (stepsLeft > 0 &&
         (!limitSignal || signalIsInLimit(moveDown, limitingSignal))) {
    if (!stepPiezo(direction * stepIncrement)) {
      break;
    }
    stepsLeft -= stepIncrement;
  }
  return stepsLeft;
}

long Piezo::move(long stepsLeft, boolean moveDown,
                 boolean limitSignal,
                 float limitingSignal /* V */,
                 long stepIncrement) { // fixme: rename to displace?
  if (!limitSignal) {
    int direction = moveDown ? 1 : -1;
    stepPiezo(direction * stepsLeft);
    return 0;
  }

  return singleStepPiezo(stepsLeft, moveDown, limitSignal, limitingSignal,
                         stepIncrement);
}

void Piezo::down(String &parameters) {
  String s;
  long steps, stepsLeft, stepIncrement = 1;
  float maxSignal = 0;
  boolean limitSignal = false;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if ((s = shift(parameters)) != "") {
    maxSignal = s.toFloat();
    limitSignal = true;
  }

  if ((s = shift(parameters)) != "") {
    stepIncrement = s.toInt();
  }

  stepsLeft = movePiezo(steps, true, limitSignal, maxSignal, stepIncrement);

  printSummary(steps - stepsLeft);
}

void piezo::up(String &parameters) {
  String s;
  long steps, stepsLeft, stepIncrement = 1;
  float minSignal = 0;
  boolean limitSignal = false;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if ((s = shift(parameters)) != "") {
    minSignal = s.toFloat();
    limitSignal = true;
  }

  if ((s = shift(parameters)) != "") {
    stepIncrement = s.toInt();
  }

  stepsLeft = movePiezo(steps, false, limitSignal, minSignal, stepIncrement);
  printSummary(steps - stepsLeft);
}

boolean Piezo::step(int step) {
  if ((piezoPosition == 0xffff && step > 0) ||
      (piezoPosition == 0 && step < 0)) {
    return false; // not stepped
  }
  piezoPosition += step;
  if (piezoPosition < 0) {
    piezoPosition = 0;
  } else if (piezoPosition > 0xffff) {
    piezoPosition = 0xffff;
  }

  positionPiezo();
  return true;
}

void Piezo::moveAllTheWayUp() {
  movePiezo(0xffff, false, false, 0, 1);
}

#endif
