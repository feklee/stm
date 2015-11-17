#include <SPI.h>

/*
  For explanation of motor control, see:
  <http://www.instructables.com/id/BYJ48-Stepper-Motor>
*/

#define MOTOR_PIN0 2
#define MOTOR_PIN1 3
#define MOTOR_PIN2 4
#define MOTOR_PIN3 5
#define SIGNAL_PIN A2
#define BIAS_MEASURE_PIN A3

// Hard coded in SPI library: 11 = MOSI, 13 = SCK
#define PIEZO_CHIP_SELECT_PIN 7
#define BIAS_CHIP_SELECT_PIN 8

#define MAX_PIEZO_POSITION 65535

uint16_t piezoPosition = 0;
boolean signalLogIsEnabled = false;

float readVoltageWithTeensyLC(int pin) {
  return analogRead(pin) * 3.3 / 0xffff;
}

void setupPiezoSPI() {
  pinMode(PIEZO_CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(PIEZO_CHIP_SELECT_PIN, HIGH);
}

void setupBiasRegulatorSPI() {
  pinMode(BIAS_CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(BIAS_CHIP_SELECT_PIN, HIGH);
}

void setupMotor() {
  pinMode(MOTOR_PIN0, OUTPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  setupMotor();
  setupPiezoSPI();
  setupBiasRegulatorSPI();
  SPI.begin();
  setBiasVoltageFactor(0.001);
  analogReadResolution(16);
  prompt();
}

void prompt() {
  Serial.println("Enter command or `help`!");
}

void loop() {
  if (Serial.available() > 0) {
    interpretCommand(Serial.readString());
  }
}

String shift(String &s) {
  int i = s.indexOf(' ');
  String shifted, value;
  if (i == -1) {
    value = s;
    s = "";
    return value;
  }
  value = s.substring(0, i);
  s = s.substring(i + 1);
  return value;
}

void help() {
  Serial.println("Commands:\n"
                 "\n"
                 "  * enable-signal-log\n"
                 "\n"
                 "    Enables output of signal during movements.\n"
                 "\n"
                 "  * disable-signal-log\n"
                 "\n"
                 "  * down <steps> [max. signal (V)]\n"
                 "\n"
                 "    Moves tip down while current signal is below maximum.\n"
                 "\n"
                 "  * up <steps> [min. signal (V)]\n"
                 "\n"
                 "    Moves tip up while current is above minimum.\n"
                 "\n"
                 "  * piezo-down <steps> [max. signal (V)]\n"
                 "\n"
                 "  * piezo-up <steps> [min. signal (V)]\n"
                 "\n"
                 "  * set-bias <voltage (mV)>\n");
}

void interpretCommand(String s) {
  Serial.print("> ");
  Serial.println(s);

  String command = shift(s);

  if (command == "enable-signal-log") {
    signalLogIsEnabled = true;
  } else if (command == "disable-signal-log") {
    signalLogIsEnabled = false;
  } else if (command == "down") {
    down(s);
  } else if (command == "up") {
    up(s);
  } else if (command == "piezo-down") {
    piezoDown(s);
  } else if (command == "piezo-up") {
    piezoUp(s);
  } else if (command == "set-bias") {
    setBias(s);
  } else {
    help();
  }
}

void printSummary(uint16_t stepsExecuted = 0) {
  flushSignalLog();
  Serial.print("Signal (V): ");
  Serial.print(readSignal());
  Serial.print("; Steps executed: ");
  Serial.print(stepsExecuted);
  Serial.print("; Piezo position (0-65535): ");
  Serial.print(piezoPosition);
  Serial.print("; Measured bias (mV): ");
  Serial.println(1000 * measuredBias());
}

void down(String &parameters) {
  String s;
  uint16_t steps, stepsLeft;
  float maxSignal = 6; // max. voltage that Arduino can measure is 5V

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if ((s = shift(parameters)) != "") {
    maxSignal = s.toFloat();
  }

  stepsLeft = rotate(steps, false, maxSignal);
  printSummary(steps - stepsLeft);
}

void up(String &parameters) {
  String s;
  uint16_t steps, stepsLeft;
  float minSignal = -1;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if ((s = shift(parameters)) != "") {
    minSignal = s.toFloat();
  }

  stepsLeft = rotate(steps, true, minSignal);
  printSummary(steps - stepsLeft);
}

void piezoDown(String &parameters) {
  String s;
  uint16_t steps, stepsLeft;
  float maxSignal = 6; // max. voltage that Arduino can measure is 5V

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if ((s = shift(parameters)) != "") {
    maxSignal = s.toFloat();
  }

  stepsLeft = movePiezo(steps, true, maxSignal);
  printSummary(steps - stepsLeft);
}

void piezoUp(String &parameters) {
  String s;
  uint16_t steps, stepsLeft;
  float minSignal = -1;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if ((s = shift(parameters)) != "") {
    minSignal = s.toFloat();
  }

  stepsLeft = movePiezo(steps, false, minSignal);
  printSummary(steps - stepsLeft);
}

void setBias(String &parameters) {
  String s;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  setBiasVoltageFactor(s.toFloat() / 5000);
  printSummary();
}

void logSignal(float signal, boolean flush = false) {
  static boolean hasBeenFlushed = true;

  if (flush) {
    Serial.println();
    hasBeenFlushed = true;
    return;
  }

  Serial.print(hasBeenFlushed ? "" : ", ");
  Serial.print(signal);
  hasBeenFlushed = false;
}

unsigned long movePiezo(unsigned long stepsLeft, boolean moveDown,
                        float limitingSignal /* V */) {
  float signal = readAndLogSignal();
  while (stepsLeft > 0 &&
         ((moveDown && signal < limitingSignal) ||
          (!moveDown && signal > limitingSignal))) {
    if (!stepPiezo(moveDown)) {
      break;
    }
    stepsLeft --;
    signal = readAndLogSignal();
  }
  return stepsLeft;
}

boolean stepPiezo(boolean moveDown) {
  if ((piezoPosition == 0xffff && moveDown) ||
      (piezoPosition == 0 && !moveDown)) {
    return false; // not stepped
  }
  piezoPosition += moveDown ? 1 : -1;
  positionPiezo();
  return true;
}

void flushSignalLog() {
  logSignal(0, true);
}

// Voltage, proportional to tip current.
float readSignal() /* V */ {
  int sensorValue = readVoltageWithTeensyLC(SIGNAL_PIN);
  float voltage = sensorValue / 1023. * 5;
  return voltage;
}

float readAndLogSignal() {
  float signal = readSignal();
  if (signalLogIsEnabled) {
    logSignal(signal);
  }
  return signal;
}

boolean isMovingDown(boolean rotateClockwise) {
  return !rotateClockwise;
}

boolean isMovingUp(boolean rotateClockwise) {
  return !isMovingDown(rotateClockwise);
}

unsigned long rotate(unsigned long stepsLeft, boolean rotateClockwise,
                     float limitingSignal /* V */) {
  float signal = readAndLogSignal();
  while (stepsLeft > 0 &&
         ((isMovingDown(rotateClockwise) && signal < limitingSignal) ||
          (isMovingUp(rotateClockwise) && signal > limitingSignal))) {
    step(rotateClockwise);
    stepsLeft --;
    delay(1);
    signal = readAndLogSignal();
  }
  return stepsLeft;
}

void step(boolean rotateClockwise) {
  static byte position;
  sendPosition(position);
  position = nextPosition(position, rotateClockwise);
}

void sendPosition(byte position) {
  switch (position) {
  case 0:
    setMotorPins(LOW, LOW, LOW, HIGH);
    break;
  case 1:
    setMotorPins(LOW, LOW, HIGH, HIGH);
    break;
  case 2:
    setMotorPins(LOW, LOW, HIGH, LOW);
    break;
  case 3:
    setMotorPins(LOW, HIGH, HIGH, LOW);
    break;
  case 4:
    setMotorPins(LOW, HIGH, LOW, LOW);
    break;
  case 5:
    setMotorPins(HIGH, HIGH, LOW, LOW);
    break;
  case 6:
    setMotorPins(HIGH, LOW, LOW, LOW);
    break;
  case 7:
    setMotorPins(HIGH, LOW, LOW, HIGH);
    break;
  }
}

void setMotorPins(byte val0, byte val1, byte val2, byte val3) {
  digitalWrite(MOTOR_PIN0, val0);
  digitalWrite(MOTOR_PIN1, val1);
  digitalWrite(MOTOR_PIN2, val2);
  digitalWrite(MOTOR_PIN3, val3);
}

byte nextPosition(byte position, boolean rotateClockwise) {
  position += (rotateClockwise ? 1 : -1);
  return position & 7;
}

void positionPiezo() {
  digitalWrite(PIEZO_CHIP_SELECT_PIN, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  SPI.transfer((piezoPosition >> 8) & 0xff);
  SPI.transfer(piezoPosition & 0xff);
  SPI.endTransaction();
  digitalWrite(PIEZO_CHIP_SELECT_PIN, HIGH);
}

void setBiasVoltageFactor(float factor /* [0, 1] */) {
  const int iMax =  (1 << 12) - 1;
  int i = iMax * factor;
  byte b;

  digitalWrite(BIAS_CHIP_SELECT_PIN, LOW);
  SPI.beginTransaction(SPISettings(1400000, MSBFIRST, SPI_MODE0));
  b = highByte(i);
  b &= 0b00001111;
  b |= 0b00110000; // activates chip, sets gain to 1×
  SPI.transfer(b);
  b = lowByte(i);
  SPI.transfer(b);
  SPI.endTransaction();
  digitalWrite(BIAS_CHIP_SELECT_PIN, HIGH);
}

float measuredBias() /* V */ {
  return readVoltageWithTeensyLC(BIAS_MEASURE_PIN);
}
