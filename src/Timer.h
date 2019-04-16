#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "Event.h"

class Timer
{
    // Felder
public:
    Delegate<>* TimeIsUpEvent;      // Wird ausgelöst, wenn die Zeit abgelaufen ist
private:
    unsigned long Intervall;        // Zeit in ms, die der Timer laufen soll
    unsigned long StartTime;        // Systemzeit, zu der der Timer gestartet wurde
    bool IsRunning;                 // Gibt an, ob der Timer läuft

    // Konstruktor
public:
    Timer(unsigned long intervall);

    // Methoden
public:
    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Startet den Timer neu
    void Start();

    // Stoppt den Timer
    void Stop();

    // Setzt die Anfangszeit
    void SetTime(unsigned long intervall);
};

#endif
