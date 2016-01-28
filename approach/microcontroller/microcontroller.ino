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

String taggedSignal(float signal) {
  char s[11];
  dtostrf(signal, 10, 2, s);
  return "#" + String(s).trim(); // for easy parsing, e.g. for plotting
}

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
  setBiasVoltageFactor(0.01);
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
                 "  * set-bias <voltage (mV)>\n"
                 "\n"
                 "  * down <steps> [max. signal (V)]\n"
                 "\n"
                 "    Moves tip down while current signal is below maximum.\n"
                 "\n"
                 "  * up <steps> [min. signal (V)]\n"
                 "\n"
                 "    Moves tip up while current is above minimum.\n"
                 "\n"
                 "  * piezo-down <steps> [max. signal (V)] [step-increment]\n"
                 "\n"
                 "  * piezo-up <steps> [min. signal (V)] [step-increment]\n"
                 "\n"
                 "  * piezo-play\n"
                 "\n"
                 "    Plays a sound to test the piezo.\n"
                 "\n"
                 "  * woodpecker-down <max. signal (V)> <step-increment> "
                 "<piezo-step-increment>\n"
                 "\n"
                 "    Approaches automatically using the woodpecker method:\n"
                 "    motor down by `step-increment`, piezo down, if maximum "
                 "signal reached\n"
                 "    then stop or else move piezo up and repeat\n"
                 "\n"
                 "  * monitor-signal <duration (ms)> [delay (µs)]\n"
                 "\n"
                 "    Repeatedly prints the current signal (V).\n"
                 "\n"
                 "  * silently-monitor-signal <duration (ms)> [delay (µs)]\n"
                 "\n"
                 "    Repeatedly measures the current signal."
  );
}

void printElapsedTime(unsigned long startMillis) {
  Serial.print("(");
  Serial.print((millis() - startMillis) / 1000.);
  Serial.println(" s)");
}

void interpretCommand(String s) {
  Serial.print("> ");
  Serial.println(s);

  String command = shift(s);

  unsigned long startMillis = millis();

  clearSignalLog();

  if (command == "set-bias") {
    interpretSetBias(s);
  } else if (command == "down") {
    interpretDown(s);
  } else if (command == "up") {
    interpretUp(s);
  } else if (command == "piezo-down") {
    interpretPiezoDown(s);
  } else if (command == "piezo-up") {
    interpretPiezoUp(s);
  } else if (command == "piezo-play") {
    interpretPiezoPlay();
  } else if (command == "woodpecker-down") {
    interpretWoodpeckerDown(s);
  } else if (command == "monitor-signal") {
    interpretMonitorSignal(s);
  } else if (command == "silently-monitor-signal") {
    interpretSilentlyMonitorSignal(s);
  } else {
    help();
  }

  printElapsedTime(startMillis);
}

void clearSignalLog() {
  int i;
  for (i = 0; i < signalLogSize; i ++) {
    signalLog[i] = 0;
  }
  signalLogHead = 0;
  signalLogSize = 0;
}

void printLastSignals() {
  int i, j;
  String separator = "";
  Serial.print("Last signals in reverse (V): ");
  if (signalLogSize == 0) {
    Serial.println("nothing recorded");
    return;
  }
  for (i = signalLogSize - 1; i >= 0; i --) {
    j = (signalLogHead + i) % signalLogSize;
    Serial.print(separator);
    Serial.print(taggedSignal(signalLog[j]));
    separator = ", ";
  }
  Serial.println("");
}

void printSummary(long stepsMoved = 0) {
  printLastSignals();
  Serial.print("Current signal (V): ");
  Serial.print(readSignal());
  Serial.print("; Steps moved: ");
  Serial.print(stepsMoved);
  Serial.print("; Piezo position (0-65535): ");
  Serial.print(piezoPosition);
  Serial.print("; Measured bias (mV): ");
  Serial.println(1000 * measuredBias());
}

void interpretDown(String &parameters) {
  String s;
  long steps, stepsLeft;
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

void interpretUp(String &parameters) {
  String s;
  long steps, stepsLeft;
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

void interpretPiezoDown(String &parameters) {
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

void interpretPiezoUp(String &parameters) {
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

void positionPiezoForDuration(long desiredPosition, long duration /* ms */) {
  long startMillis = millis(), endMillis, waitMillis;
  piezoPosition = desiredPosition;
  positionPiezo();
  endMillis = millis();
  waitMillis = duration - (endMillis - startMillis);
  if (waitMillis <= 0) {
    return;
  }
  delay(waitMillis);
}

void interpretPiezoPlay() {
  float frequency = 440 /* Hz */;
  long duration = 1000 / frequency / 2 /* ms */,
    totalDuration = 2000 /* ms */;

  for (long i = 0; i < totalDuration / (2 * duration); i ++) {
    positionPiezoForDuration(0xffff, duration);
    positionPiezoForDuration(0, duration);
  }

  printSummary();
}

void interpretSetBias(String &parameters) {
  String s;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  setBiasVoltageFactor(s.toFloat() / 5000);
  printSummary();
}

void printSignal(float signal, boolean flush = false) {
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

boolean signalIsInLimit(boolean isMovingDown, float limitingSignal) {
  float signal = readAndLogSignal(); // always log

  if (isMovingDown) {
    return signal < limitingSignal;
  } else {
    return signal > limitingSignal;
  }
}

long movePiezo(long stepsLeft, boolean moveDown,
               boolean limitSignal,
               float limitingSignal /* V */,
               long stepIncrement) {
  if (!limitSignal) {
    int direction = moveDown ? 1 : -1;
    stepPiezo(direction * stepsLeft);
    return 0;
  }

  return singleStepPiezo(stepsLeft, moveDown, limitSignal, limitingSignal,
                         stepIncrement);
}

long singleStepPiezo(long stepsLeft, boolean moveDown,
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

boolean stepPiezo(int step) {
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

// Voltage, proportional to tip current.
float readSignal() /* V */ {
  return readVoltageWithTeensyLC(SIGNAL_MEASURE_PIN);
}

void logSignal(float signal) {
  signalLog[signalLogHead] = signal;

  signalLogHead ++;
  if (signalLogHead >= signalLogMaxSize) {
    signalLogHead = 0;
  }

  signalLogSize =
    (signalLogSize < signalLogMaxSize) ?
    signalLogSize + 1 :
    signalLogMaxSize;
}

float readAndLogSignal() {
  float signal = readSignal();
  logSignal(signal);
  return signal;
}

boolean isMovingDown(boolean rotateClockwise) {
  return !rotateClockwise;
}

long rotate(long stepsLeft, boolean rotateClockwise,
            float limitingSignal /* V */) {
  while (stepsLeft > 0 && signalIsInLimit(isMovingDown(rotateClockwise),
                                          limitingSignal)) {
    step(rotateClockwise);
    stepsLeft --;
    delay(1);
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
  delayMicroseconds(100); // time to dampen noise from SPI communication, which
                          // can get picked up as signal, see
                          // <https://github.com/feklee/stm/issues/3>
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

void movePiezoAllTheWayUp() {
  movePiezo(0xffff, false, false, 0, 1);
}

void interpretWoodpeckerDown(String &parameters) {
  String
    s1 = shift(parameters),
    s2 = shift(parameters),
    s3 = shift(parameters);
  long stepIncrement, piezoStepIncrement;
  float maxSignal;
  long stepsLeft;

  if (s3 == "") {
    help();
    return;
  }

  maxSignal = s1.toFloat();
  stepIncrement = s2.toInt();
  piezoStepIncrement = s3.toInt();

  Serial.println("After each motor step, the log of signals is cleared.");
  while (true) {
    Serial.print("Signal before next motor step (V): ");
    Serial.print(readSignal());
    Serial.println();

    movePiezoAllTheWayUp();
    rotate(stepIncrement, false, maxSignal);
    clearSignalLog();
    stepsLeft = movePiezo(0xffff, true, true, maxSignal, piezoStepIncrement);
    if (stepsLeft >= piezoStepIncrement) {
      break; // stopped because signal is too large
    }
  }

  printSummary();
}

void interpretMonitorSignal(String &parameters) {
  String s;
  unsigned long duration, measurementDelay = 100000;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }
  duration = s.toInt();

  if ((s = shift(parameters)) != "") {
    measurementDelay = s.toInt();
  }

  monitorSignal(duration, measurementDelay, true);
}

void interpretSilentlyMonitorSignal(String &parameters) {
  String s;
  unsigned long duration, measurementDelay = 100000;

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }
  duration = s.toInt();

  if ((s = shift(parameters)) != "") {
    measurementDelay = s.toInt();
  }

  monitorSignal(duration, measurementDelay, false);
}

void monitorSignal(unsigned long duration /* ms */,
                   unsigned long measurementDelay /* µs */,
                   boolean signalShouldBePrinted) {
  unsigned long startMillis = millis(), endMillis;

  endMillis = startMillis + duration;
  while (millis() < endMillis) {
    if (signalShouldBePrinted) {
      Serial.print(readSignal()); // fixme
      Serial.print(", ");
      float x = readSignal();
      Serial.print(taggedSignal(x));
      Serial.print(", ");
      Serial.println(taggedSignal(0.0)); // fixme
    } else {
      readAndLogSignal();
    }
    delayMicroseconds(measurementDelay);
  }

  printSummary();
}
