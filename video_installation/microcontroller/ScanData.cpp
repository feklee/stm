#include <Arduino.h>
#include <ArduinoJson.h>
#include "ScanData.hpp"

void ScanData::printJson() {
  const int bufferSize = JSON_OBJECT_SIZE(2) +
    JSON_ARRAY_SIZE(chunkSize) + chunkSize * JSON_ARRAY_SIZE(5);
  StaticJsonBuffer<bufferSize> jsonBuffer;

  JsonObject &root = jsonBuffer.createObject();
  root["type"] = "scan data";

  JsonArray &d = root.createNestedArray("data");

  for (int i = 0; i < head; i ++) {
    ScanDatum &scanDatum = data[i];
    JsonArray &datum = d.createNestedArray();
    datum.add(scanDatum.x);
    datum.add(scanDatum.y);
    datum.add(scanDatum.z);
    datum.add(scanDatum.voltage);
    datum.add(scanDatum.timestamp);
  }

  root.printTo(Serial);
}

void ScanData::flushScanData() {
  printJson();
  Serial.println();
  head = 0;
}

void ScanData::append(ScanDatum d) {
  data[head] = d;
  head ++;
  if (head >= chunkSize) {
    flushScanData();
  }
}
