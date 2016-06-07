// Arduino / Teensy sketch

void setup() {
  pinMode(1, OUTPUT);
}

float readVoltageWithTeensyLC(int pin) {
  return analogRead(pin) * 3.3 / 0xffff;
}

void loop() {
  Serial.println(readVoltageWithTeensyLC(1));
  delay(100);
}
