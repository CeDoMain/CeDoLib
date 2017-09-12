#ifndef Encoder_H
#define Encoder_H

#include "Arduino.h"
#include "Event.h"
#include "Trigger.h"

class Encoder
{
    // Felder
  private:
    Trigger* TrigA;           // Trigger, der Spur A darstellt
    Trigger* TrigB;           // Trigger, der Spur B darstellt
    bool ValueA, ValueB;      // Wert der beiden Trigger
    long Counter;             // Zählerstand
    byte SubCount;            // relative Impulszahl zum Zählerstand
    const byte Divider;       // Impulse, die einen Zählimpuls ergeben

  public:
    Event<long> CounterChangedEvent;      // Wird ausgelöst, wenn sich der Zähler geändert hat
    Event<long> CountUpEvent;             // Wird ausgelöst, wenn der Zähler nach oben zählt
    Event<long> CountDownEvent;           // Wird ausgelöst, wenn der Zähler nach unten zählt

    // Konstruktor
  public:
    Encoder(const byte divider);

    // Methoden
  public:
    // Initialisiert den Encoder
    void Begin(Trigger* triggerA, Trigger* triggerB);

    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Setzt den Zähler zurück
    void Reset(long newValue = 0);

    // Gibt den aktuellen Zählerstand zurück
    long GetCount();

  private:
    // Wird aufgerufen, wenn der Trigger A aktiviert wird
    void ActivateA();

    // Wird aufgerufen, wenn der Trigger A deaktiviert wird
    void DeactivateA();

    // Wird aufgerufen, wenn der Trigger B aktiviert wird
    void ActivateB();

    // Wird aufgerufen, wenn der Trigger B deaktiviert wird
    void DeactivateB();

  private:
    // Zähler +1
    void CountUp();

    // Zähler -1
    void CountDown();
};

#endif
