#include <Arduino.h>
#include "CurrentLog.hpp"

void CurrentLog::print() {
  Serial.print("{\"currentSignals\":[");
  for (int i = 0; i < head_; i ++) {
    if (i > 0) {
      Serial.print(",");
    }
    Serial.print(currentSignals_[i]);
  }
  Serial.println("]}");
}

void CurrentLog::flush() {
  if (head_ > 0) {
    print();
    head_ = 0;
  }
}

void CurrentLog::add(Current &current) {
  currentSignals_[head_] = current.signal();
  head_ ++;
  if (head_ >= size_) {
    flush();
  }
}
