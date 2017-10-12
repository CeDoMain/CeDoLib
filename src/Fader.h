#ifndef FADER_H
#define FADER_H

#include "Event.h"

typedef short decimal;

template<typename T> class Fader
{
    // Felder
public:
    Delegate<void, T>* FadeEvent; // Wird bei jedem Fade-Schritt aufgerufen

private:
    bool IsFading;                // Gibt an, ob gefadet wird
    T From, To, Value;            // Intervall in dem gefadet wird [From;To] und aktueller Wert
    unsigned short FadeTime;      // Zeit in ms, die für den Fade benötigt wird
    unsigned long FadeStartTime;  // Zeit in ms, zu der der Fade gestartet wurde

    // Konstruktor
public:
    Fader()
        : FadeEvent(0), IsFading(false), From(), To(), Value(),
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
            
        // Erzeugt einen linearen Verlauf von 1 nach 0
        decimal f = 10000 - ((millis() - FadeStartTime) % FadeTime) * 10;
      
        if (millis() - FadeStartTime > FadeTime)
        {
            f = 0;
            IsFading = false;
        }
        Value = f * From / 10000 + (10000 - f) * To / 10000;
        if (FadeEvent != 0)
            (*FadeEvent)(Value);
    }

    // Startet den Fade vom aktuellen Wert zum neuen Ziel
    void Start(unsigned short fadeTime, T to)
    {
        Start(fadeTime, Value, to);
    }

    // Startet den Fade im angegebenen Intervall
    void Start(unsigned short fadeTime, T from, T to)
    {
        if(from == to)
        {
            if(Value != To)
            {
                Value = To;
                if (FadeEvent != 0)
                    (*FadeEvent)(To);
            }
            return;
        }
        FadeTime = fadeTime;
        From = from;
        To = to;
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
