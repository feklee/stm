#include <SPI.h> // hard coded pins: 11 = MOSI, 13 = SCK
#include "util.hpp"
#include "Piezo.hpp"

void Piezo::setup() {
  pinMode(chipSelectPin_, OUTPUT);
  digitalWrite(chipSelectPin_, HIGH);
}

void Piezo::sendDisplacement() {
  byte dacChannel = 0;
  digitalWrite(chipSelectPin_, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  SPI.transfer(dacChannel);
  SPI.transfer16(displacement_);
  SPI.endTransaction();
  digitalWrite(chipSelectPin_, HIGH);
}

void Piezo::displaceForDuration(uint16_t displacement,
                                unsigned long duration /* ms */) {
  unsigned long startTime = millis();
  displacement_ = displacement;
  sendDisplacement();
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

void Piezo::displace(uint16_t displacement) {
  displacement_ = displacement;
  sendDisplacement();
}

void Piezo::addToDisplacement(uint16_t delta) {
  if (delta > 0) {
    displacement_ =
      (0xffff - displacement_ < delta) ?
      0xffff :
      displacement_ + delta;
  } else {
    displacement_ =
      (displacement_ < delta) ?
      0 :
      displacement_ + delta;
  }
}

void Piezo::displaceByDelta(uint16_t delta) {
  addToDisplacement(delta);
  sendDisplacement();
}

uint16_t Piezo::displacement() {
  return displacement_;
}
