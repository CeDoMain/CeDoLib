#ifndef FADER_H
#define FADER_H

#include "Arduino.h"
#include "Event.h"

typedef short decimal;

class Fader
{
    // Felder
public:
    Delegate<void, decimal>* FadeEvent; // Wird bei jedem Fade-Schritt aufgerufen

private:
    bool IsFading;                      // Gibt an, ob gefadet wird
    decimal From, To, Value;            // Intervall in dem gefadet wird [From;To] und aktueller Wert
    unsigned short FadeTime;            // Zeit in ms, die für den Fade benötigt wird
    unsigned long FadeStartTime;        // Zeit in ms, zu der der Fade gestartet wurde

    // Konstruktor
public:
    Fader();

    // Methoden
public:
    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Startet den Fade vom aktuellen Wert zum neuen Ziel
    void Start(unsigned short fadeTime, decimal to);

    // Startet den Fade im angegebenen Intervall
    void Start(unsigned short fadeTime, decimal from, decimal to);

    // Stoppt den Fade
    void Stop();
};

#endif
