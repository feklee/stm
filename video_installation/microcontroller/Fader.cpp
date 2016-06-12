// Tracks position of fader from "Real" to "Ideal".

#include "util.hpp"
#include "Fader.hpp"

void Fader::print() {
  printValue("faderUpate", readVoltage(potPin_));
}
