#include <SPI.h>

#define MOSI 11 // Anschlüsse SPI BUS und Chip Select
#define CL 13
#define CLOCK digitalWrite(CL,1); digitalWrite(CL,0);
#define CHIP_SELECT_PIN 7 // hard coded in SPI library: 11 = MOSI, 13 = SCK

void setupSPI() {
  pinMode(CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(CHIP_SELECT_PIN, LOW);

//  SPI.begin();

  pinMode(MOSI,OUTPUT); // alles Ausgänge
  digitalWrite(MOSI, LOW);
  pinMode(SCK,OUTPUT);
  digitalWrite(SCK, LOW);
}

void setup() {
  Serial.begin(9600);
  setupSPI();
}

void setBiasVoltageFactor(float factor /* [0,1] */) {
  const int iMax =  (1 << 12) - 1;
  int i = iMax * factor;

  digitalWrite(CHIP_SELECT_PIN, LOW);

  sendHeader();             // Header senden
//  SPI.transfer(i & 0xff);

  for(int j=11;j>=0;j--){   // Daten senden
    digitalWrite(MOSI,((i&(1<<j)))>>j);
    CLOCK
  }
  digitalWrite(CHIP_SELECT_PIN,1);      // Chip abwählen
}

void loop() {
  for (float x = 0; x < 1; x += 0.0001)
    setBiasVoltageFactor(x);
}

void sendHeader() {
// Vorwort laut Datenblatt Seite 18

  digitalWrite(MOSI,0); // 0: auf MOSIC0 ausgeben
  CLOCK
  digitalWrite(MOSI,0); // ungepuffert
  CLOCK
  digitalWrite(MOSI,1); // Gain: 1=1x 0=2x
  CLOCK
  digitalWrite(MOSI,1); // Chip aktiv
  CLOCK
}
