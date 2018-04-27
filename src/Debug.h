#ifndef DEBUG_H
#define DEBUG_H

#include "stdarg.h"
#include "Arduino.h"
#include "MemoryFree.h"

// Code von http://playground.arduino.cc/Main/Printf

// PrintF mit Formatstring aus dem RAM und Ausgabe im Serial Port
void Debug(char* fmt, ... );

// PrintF mit Formatstring aus dem Flash und Ausgabe im Serial Port
void Debug(const __FlashStringHelper *fmt, ... );

// Schickt den Anteil des belegten RAMs im Serial Port an
void PrintRAMUsage();

#endif
