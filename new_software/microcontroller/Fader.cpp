// Tracks position of fader from "Real" to "Ideal".

#include "util.hpp"
#include "Fader.hpp"

void Fader::print() {
  float value = float(analogRead(potPin_)) / 0xffff; // [0, 1]
  printValue("faderPosition", value);
}
