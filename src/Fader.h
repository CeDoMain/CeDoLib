#ifndef FADER_H
#define FADER_H

#include "Event.h"

typedef short decimal;

class Fader
{
    // Felder
public:
    Delegate<void, decimal>* FadeEvent; // Wird bei jedem Fade-Schritt aufgerufen

private:
    bool IsFading;                // Gibt an, ob gefadet wird
    decimal From, To, Value;      // Intervall in dem gefadet wird [From;To] und aktueller Wert
    unsigned short FadeTime;      // Zeit in ms, die für den Fade benötigt wird
    unsigned long FadeStartTime;  // Zeit in ms, zu der der Fade gestartet wurde

    // Konstruktor
public:
    Fader()
        : FadeEvent(0), IsFading(false), From(0), To(0), Value(0),
        FadeTime(0), FadeStartTime(0)
    {
        
    }

    // Methoden
public:
    // Muss in jedem Frame aufgerufen werden
    void Update()
    {
        if (!IsFading)
            return;
            
        // Erzeugt einen linearen Verlauf von 0 nach 1
        decimal f = (millis() - FadeStartTime) * 10000 / FadeTime;
      
        if (millis() - FadeStartTime > FadeTime)
        {
            f = 10000;
            IsFading = false;
        }
        Value = map(f, 0, 10000, From, To);
        if (FadeEvent != 0)
            (*FadeEvent)(Value);
    }

    // Startet den Fade vom aktuellen Wert zum neuen Ziel
    void Start(unsigned short fadeTime, decimal to)
    {
        Start(fadeTime, Value, to);
    }

    // Startet den Fade im angegebenen Intervall
    void Start(unsigned short fadeTime, decimal from, decimal to)
    {
        FadeTime = fadeTime;
        From = from;
        To = to;
        if(From == To)
        {
            if(Value != To)
            {
                Value = To;
                if (FadeEvent != 0)
                    (*FadeEvent)(To);
            }
            return;
        }
        FadeStartTime = millis();
        IsFading = true;
    }

    // Stoppt den Fade
    void Stop()
    {
        IsFading = false;
    }
};

#endif
