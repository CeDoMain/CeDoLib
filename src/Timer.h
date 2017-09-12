#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "Event.h"

class Timer
{
  // Felder
private:
  unsigned int intervall;
  unsigned long startTime;
  bool isRunning;
  bool isEndless;

  // Funktionszeiger
public:
  Event<> TimeIsUpEvent;

  // Konstruktor
public:
  Timer(unsigned int intervall, bool isEndless = false, bool isRunning = false);

  // Methoden
public:
  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Startet den Timer neu
  void Start();

  // Stoppt den Timer
  void Stop();

  // Setzt die Anfangszeit
  void SetTime(unsigned int intervall);
};

#endif
