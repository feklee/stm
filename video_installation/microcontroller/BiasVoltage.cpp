#include <SPI.h> // hard coded pins: 11 = MOSI, 13 = SCK
#include "util.hpp"
#include "BiasVoltage.hpp"

void BiasVoltage::setup() {
  pinMode(chipSelectPin_, OUTPUT);
  digitalWrite(chipSelectPin_, HIGH);
}

void BiasVoltage::setFactor(float factor /* [0, 1] */) {
  const int iMax =  (1 << 12) - 1;
  int i = iMax * factor;
  byte b;

  digitalWrite(chipSelectPin_, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  b = highByte(i);
  b &= 0b00001111;
  b |= 0b00110000; // activates chip, sets gain to 1×
  SPI.transfer(b);
  b = lowByte(i);
  SPI.transfer(b);
  SPI.endTransaction();
  digitalWrite(chipSelectPin_, HIGH);
}

void BiasVoltage::set(float voltage /* mV */) {
  setFactor(voltage / 3300);
}

float BiasVoltage::measuredValue() { // mV
  return 1000 * readVoltage(measurePin_);
}

void BiasVoltage::print() {
  printValue("biasVoltage", measuredValue());
}
