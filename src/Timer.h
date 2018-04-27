#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "Event.h"

class Timer
{
    // Felder
public:
    Delegate<>* TimeIsUpEvent;
private:
    unsigned long Intervall;
    unsigned long StartTime;
    bool IsRunning;

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
