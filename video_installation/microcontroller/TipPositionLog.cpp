// Positions of the tip, i.e. x, y, z, and current signal at bias voltage

#include <Arduino.h>
#include "util.hpp"
#include "TipPositionLog.hpp"

void TipPositionLog::print() {
  char buffer[8];
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
    dtostrf(entry.currentSignal, 0, 2, buffer);
    Serial.print(buffer);
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

void TipPositionLog::add(Entry &entry) {
  entries_[head_] = entry_;
  head_ ++;
  if (head_ >= size_) {
    flush();
  }
}
