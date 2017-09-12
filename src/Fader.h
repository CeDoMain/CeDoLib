#ifndef FADER_H
#define FADER_H

#include "Event.h"

class Fader
{
  // Felder
public:
  Event<float> FadeEvent;       // Wird während des Fadens sehr oft ausgelöst

private:
  bool IsFading;                // Gibt an, ob gefadet wird
  float From, To, Value;        // Intervall in dem gefadet wird [From;To] und aktueller Wert
  float FadeTime;               // Zeit in s, die für den Fade benötigt wird
  unsigned long FadeStartTime;  // Zeit in ms, zu der der Fade gestartet wurde

  // Konstruktor
public:
  Fader();

  // Methoden
public:
  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Startet den Fade von 0 nach 1
  void Start(float fadeTime);

  // Startet den Fade vom aktuellen Wert zum neuen Ziel
  void Start(float fadeTime, float to);

  // Startet den Fade im angegebenen Intervall
  void Start(float fadeTime, float from, float to);

  // Stoppt den Fade
  void Stop();
};

#endif
