// Positions of the tip, i.e. x, y, z, and current signal at bias voltage

#include <Arduino.h>
#include "TipPositionLog.hpp"

void TipPositionLog::print() {
  Serial.print("{\"type\":\"tipPositionLog\",\"positions\":[");
  for (int i = 0; i < head_; i ++) {
    Entry &entry = entries_[i];
    if (i > 0) {
      Serial.print(",");
    }
    Serial.print("[");
    Serial.print(entry.x);
    Serial.print(",");
    Serial.print(entry.y);
    Serial.print(",");
    Serial.print(entry.z);
    Serial.print(",");
    Serial.print(entry.currentSignal);
    Serial.print(",");
    Serial.print(entry.time);
    Serial.print("]");
  }
  Serial.println("]}");
}

void TipPositionLog::flush() {
  if (head_ > 0) {
    print();
    head_ = 0;
  }
}

void TipPositionLog::add(uint8_t x, uint8_t y, uint16_t z,
                         uint16_t currentSignal) {
  entries_[head_] = {x, y, z, currentSignal, micros()};
  head_ ++;
  if (head_ >= size_) {
    flush();
  }
}
