#pragma once

#include <Arduino.h>

void printError(const char *);
float readVoltage(uint8_t);
template <typename T> void printValue(const char *, T);
