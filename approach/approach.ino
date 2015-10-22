/*
  Inspired by: http://www.instructables.com/id/BYJ48-Stepper-Motor

  (C) 2015 by STM project at Fab Lab Berlin / Science Hack Days, released under
  the WTFPL <http://www.wtfpl.net/>.
*/

#define IN0 8
#define IN1 9
#define IN2 10
#define IN3 11

void setup() {
  Serial.begin(9600);
  pinMode(IN0, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  prompt();
}

void prompt() {
  Serial.println("Send number of steps: positive = CW, negative = CCW");
}

void loop() {
  String s;
  int steps;

  if (Serial.available() > 0) {
    s = Serial.readString();
    Serial.println(s.toInt());
    steps = s.toInt();
    rotate(abs(steps), steps > 0);
    prompt();
  }
}

void rotate(unsigned long stepsLeft, boolean rotateClockwise) {
  while (stepsLeft > 0) {
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
  digitalWrite(IN0, val0);
  digitalWrite(IN1, val1);
  digitalWrite(IN2, val2);
  digitalWrite(IN3, val3);
}

byte nextPosition(byte position, boolean rotateClockwise) {
  position += (rotateClockwise ? 1 : -1);
  return position & 7;
}
