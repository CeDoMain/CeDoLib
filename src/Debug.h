#ifndef DEBUG_H
#define DEBUG_H

#include "stdarg.h"
#include "Arduino.h"
#include "MemoryFree.h"

// Code von http://playground.arduino.cc/Main/Printf

void Debug(char* fmt, ... );

void Debug(const __FlashStringHelper *fmt, ... );

void PrintRAMUsage();

#endif
