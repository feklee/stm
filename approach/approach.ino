/*
  For explanation of motor control, see:
  <http://www.instructables.com/id/BYJ48-Stepper-Motor>
*/

#define MOTOR_PIN0 2
#define MOTOR_PIN1 3
#define MOTOR_PIN2 4
#define MOTOR_PIN3 5
#define SIGNAL_PIN A2

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PIN0, OUTPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
  prompt();
}

void prompt() {
  Serial.println("Enter command.");
}

void loop() {
  String s;
  int steps_requested, steps_left;
  float maxSignal = 2 /* V */;

  if (Serial.available() > 0) {
    interpretCommand(Serial.readString());
    prompt();
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
                 "  * down <steps> [while below <signal>]\n"
                 "\n"
                 "  * up <steps> [while above <signal>]");
}

void interpretCommand(String s) {
  String command = shift(s);

  if (command == "down") {
    down(s);
  } else {
    help();
  }
}

void down(String &parameters) {
  String s;
  int steps, steps_left;
  float maxSignal = 5; // max. voltage that Arduino can measure

  if ((s = shift(parameters)) == "") {
    help();
    return;
  }

  steps = s.toInt();

  if (shift(parameters) == "while" &&
      shift(parameters) == "below" &&
      (s = shift(parameters)) != "") {
    maxSignal = s.toFloat();
  }

  steps = abs(s.toInt()); // overflow possible
  steps_left = rotate(steps, false, maxSignal);

  Serial.print("Signal (V): ");
  Serial.println(signal());
  Serial.print("Steps executed: ");
  Serial.println(steps - steps_left);
}

// Voltage, proportional to tip current.
float signal() {
  int sensorValue = analogRead(SIGNAL_PIN);
  float voltage = sensorValue / 1023. * 5;
  return voltage;
}

boolean isApproaching(boolean rotateClockwise) {
  return !rotateClockwise;
}

boolean isApproachingTooClose(float maxSignal, boolean rotateClockwise) {
  if (!isApproaching(rotateClockwise)) {
    return false;
  }
  return signal() >= maxSignal;
}

unsigned long rotate(unsigned long stepsLeft, boolean rotateClockwise,
                     float maxSignal /* V */) {
  while (stepsLeft > 0 &&
         !isApproachingTooClose(maxSignal, rotateClockwise)) {
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
  byte bytes[4];
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
