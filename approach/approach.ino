/*
  For explanation of motor control, see:
  <http://www.instructables.com/id/BYJ48-Stepper-Motor>
*/

#define MOTOR_PIN0 8
#define MOTOR_PIN1 9
#define MOTOR_PIN2 10
#define MOTOR_PIN3 11
#define SIGNAL_PIN A0

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PIN0, OUTPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
  prompt();
}

void prompt() {
  Serial.println("Send number of steps: positive = CW, negative = CCW");
}

void loop() {
  String s;
  int steps;
  float maxSignal = 1 /* V */;

  if (Serial.available() > 0) {
    s = Serial.readString();
    s.trim();
    Serial.println(s.toInt());
    steps = abs(s.toInt()); // overflow possible
    rotate(abs(steps), s.charAt(0) != '-', maxSignal);
    Serial.print("Signal (V): ");
    Serial.println(signal());
    prompt();
  }
}

// Voltage, proportional to tip current.
float signal() {
  int sensorValue = analogRead(SIGNAL_PIN);
  float voltage = sensorValue * 5 / 1023;
  return voltage;
}

void rotate(unsigned long stepsLeft, boolean rotateClockwise,
            float maxSignal /* V */) {
  while (stepsLeft > 0 && signal() < maxSignal) {
    step(rotateClockwise);
    stepsLeft --;
    delay(1);
  }
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

void setPins(byte val0, byte val1, byte val2, byte val3) {
  digitalWrite(MOTOR_PIN0, val0);
  digitalWrite(MOTOR_PIN1, val1);
  digitalWrite(MOTOR_PIN2, val2);
  digitalWrite(MOTOR_PIN3, val3);
}

byte nextPosition(byte position, boolean rotateClockwise) {
  position += (rotateClockwise ? 1 : -1);
  return position & 7;
}
