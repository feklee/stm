// Arduino / Teensy sketch
//
// For explanation of motor control, see:
// <http://www.instructables.com/id/BYJ48-Stepper-Motor>

#include <SPI.h>

#define MOTOR_PIN0 2
#define MOTOR_PIN1 3
#define MOTOR_PIN2 4
#define MOTOR_PIN3 5
#define SIGNAL_MEASURE_PIN A2
#define BIAS_MEASURE_PIN A3

// Hard coded in SPI library: 11 = MOSI, 13 = SCK
#define PIEZO_CHIP_SELECT_PIN 7
#define BIAS_CHIP_SELECT_PIN 8

#define MAX_PIEZO_POSITION 65535

long piezoPosition = 0;
const long signalLogMaxSize = 1024;
float signalLog[signalLogMaxSize];
long signalLogHead = 0;
long signalLogSize = 0;

void setupPiezoSPI() {
  pinMode(PIEZO_CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(PIEZO_CHIP_SELECT_PIN, HIGH);
}

void setup() {
  Serial.begin(9600);
  setupPiezoSPI();
  SPI.begin();
  analogReadResolution(16);
}

void loop() {
  sawTooth();
}

void sawTooth() {
  while (true) {
    for (piezoPosition = 0; piezoPosition < 0xffff; piezoPosition ++) {
      positionPiezo();
      delayMicroseconds(0);
    }
  }
}

void positionPiezo() {
  digitalWrite(PIEZO_CHIP_SELECT_PIN, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0b00000000);
  SPI.transfer16(piezoPosition);
  SPI.endTransaction();
  digitalWrite(PIEZO_CHIP_SELECT_PIN, HIGH);
}
