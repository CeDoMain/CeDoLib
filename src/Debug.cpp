#include "Debug.h"

#ifdef ARDUINO_AVR_MEGA2560

// Code von http://playground.arduino.cc/Main/Printf

void Debug(char* fmt, ... )
{
    // PrintF mit Formatstring aus dem RAM und Ausgabe im Serial Port
    char buf[128];
    va_list args;
    va_start(args, fmt );
    vsnprintf(buf, 128, fmt, args);
    va_end(args);
    Serial.print(buf);
}

void Debug(const __FlashStringHelper* fmt, ... )
{
    // PrintF mit Formatstring aus dem Flash und Ausgabe im Serial Port
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(buf, sizeof(buf), (char*)fmt, args);
    va_end(args);
    Serial.print(buf);
}

void PrintRAMUsage()
{
    // Schickt den Anteil des belegten RAMs mit zwei Nachkommastellen im Serial Port an
    double mem = 100.0 - freeMemory() / 8186.0 * 100.0;
    Debug(F("%i.%i%% RAM used\n"), (int)mem, (int)((mem - (int)mem) * 100));
}

#endif
