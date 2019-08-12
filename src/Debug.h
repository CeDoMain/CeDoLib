#ifndef DEBUG_H
#define DEBUG_H

#include "stdarg.h"
#include "Arduino.h"
#include "LinkedList.h"

class SerialToValue
{
    // Datentypen
    struct Data
    {
        char Key;                           // Schlüssel zum Wert
        long Value;                         // Wert
        Delegate<>* ValueChangedEvent;      // Wird ausgelöst, wenn sich der Wert ändert
        Data(char key, long value, Delegate<>* event)
            : Key(key), Value(value), ValueChangedEvent(event) { }
    };

    // Felder
private:
    static LinkedList<Data*> Values;

    // Methoden
public:
    // Gibt den Wert zu einem Schlüssel zurück
    static long GetValue(char key);

    // Setzt den Wert zu einem Schlüssel
    static void SetValue(char key, long value);

    // Erzeugt (bzw. ändert) einen Wert mit Eventhandler (wird nicht aufgerufen)
    static void CreateValue(char key, long value, Delegate<>* event = 0);

    // Muss in jedem Frame aufgerufen werden
    static void Update();

private:
    // Ließt Asyncron (pro Aufruf 1 Zeichen) eine Zeile vom Serial Port
    static char* SerialReadLineAsync();
};

void Chrono(char prefix);


// nachfolgender Code von http://playground.arduino.cc/Main/Printf

// PrintF mit Formatstring aus dem RAM und Ausgabe im Serial Port
void Debug(char* fmt, ... );

#ifdef ARDUINO_AVR_MEGA2560

#include "MemoryFree.h"

// PrintF mit Formatstring aus dem Flash und Ausgabe im Serial Port
void Debug(const __FlashStringHelper *fmt, ... );

// Schickt den Anteil des belegten RAMs im Serial Port an
void PrintRAMUsage();

#endif

#endif
