#include "Fader.h"

Fader::Fader()
    : FadeEvent(0), IsFading(false), From(0), To(0), Value(0),
    FadeTime(0), FadeStartTime(0)
{
    
}

void Fader::Update()
{
    if (!IsFading)
        return;
        
    // Erzeugt einen linearen Verlauf von 0 nach 1
    decimal f = (millis() - FadeStartTime) * 10000 / FadeTime;
    
    if (millis() - FadeStartTime > FadeTime)
    {
        // Fade ist zuende
        f = 10000;
        IsFading = false;
    }
    Value = map(f, 0, 10000, From, To);

    // Wenn das Event verknüpft ist, auslösen
    if (FadeEvent != 0)
        (*FadeEvent)(Value);
}

void Fader::Start(unsigned short fadeTime, decimal to)
{
    // Startet den Fade vom aktuellen Wert zum neuen Ziel
    Start(fadeTime, Value, to);
}

void Fader::Start(unsigned short fadeTime, decimal from, decimal to)
{
    // Startet den Fade im angegebenen Intervall
    FadeTime = fadeTime;
    From = from;
    To = to;

    // Optimierungen prüfen
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

void Fader::Stop()
{
    // Stoppt den Fade
    IsFading = false;
}