#ifndef TRIGGER_H
#define TRIGGER_H

#include "Arduino.h"
#include "BaseIO.h"
#include "Delegate.h"
#include "Event.h"

class Trigger
{
    // Felder
  public:
    Delegate<> Activated;             // Wird aufgerufen, wenn der Trigger aktiviert wird
    Delegate<> Deactivated;           // Wird aufgerufen, wenn der Trigger deaktiviert wird
    static Event<> AnyActiveEvent;    // Wird ausgelöst, wenn irgendein Trigger aktiviert wird

  private:
    DigitalIn* Pin;                           // Eingang für diesen Trigger
    const bool IsInverted;                    // Gibt an, ob die Aktivierung umgekehrt wird
    bool LastCheckedValue;                    // Letzter Wert, der registriert wurde
    bool LastMeasuredValue;                   // Letzter Wert, der gemessen wurde (Vergleichsmessung)
    unsigned long LastMeasurementTimeStamp;   // Zeit, zu der der letzte Wert gemessen wurde
    const unsigned int DwellTime;             // Zeit in ms, die ein Wert gehalten werden muss um registriert zu werden

    // Konstruktor
  public:
    Trigger(DigitalIn* pin, const bool isInverted = true, const unsigned int dwellTime = 50);

    // Methoden
  public:
    // Muss in jedem Frame aufgerufen werden
    void Update();
};

#endif
