#include <SPI.h>

// See also: http://tronixstuff.com/2011/06/15/tutorial-arduino-and-the-spi-bus-part-ii/

#define MOSI 11 // Anschlüsse SPI BUS und Chip Select
#define CL 13
#define CLOCK digitalWrite(CL,1); digitalWrite(CL,0);
#define CHIP_SELECT_PIN 7 // hard coded in SPI library: 11 = MOSI, 13 = SCK

void setupSPI() {
  SPI.begin();
  pinMode(CHIP_SELECT_PIN, OUTPUT);
  delay(100); // probably unnecessary
}

void setup() {
  Serial.begin(9600);
  setupSPI();
}

void setBiasVoltageFactor(float factor /* [0, 1] */) {
  const int iMax =  (1 << 12) - 1;
  int i = iMax * factor;
  byte b;

  digitalWrite(CHIP_SELECT_PIN, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  b = highByte(i);
  b &= 0b00001111;
  b |= 0b00110000; // activates chip, sets gain to 1×
  SPI.transfer(b);
  b = lowByte(i);
  SPI.transfer(b);
  SPI.endTransaction();
  digitalWrite(CHIP_SELECT_PIN, HIGH);
}

void loop() {
  for (float x = 0; x < 1; x += 0.0001) {
    setBiasVoltageFactor(x);
  }
}
