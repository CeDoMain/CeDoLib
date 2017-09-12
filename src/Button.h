#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Event.h"
#include "Trigger.h"

class Button
{
    // Felder
  private:
    Trigger* Trig;                        // Trigger, der den Button darstellt
    const unsigned int ClickTime;         // Zeit in ms, die maximal zwischen drücken und loslassen vergehen darf, um einen Click auszulösen
    const unsigned int LongPressTime;     // Zeit in ms, die der Button für einen LongPress minimal gedrückt sein muss
    unsigned long ActivatedTimeStamp;     // Zeit in ms, zu der der Button das letzte Mal gedrückt wurde
    unsigned long DeactivatedTimeStamp;   // Zeit in ms, zu der der Button das letzte Mal losgelassen wurde
    unsigned long LastClickTimeStamp;     // Zeit in ms, zu der der letzte Klick ausgeführt wurde
    bool LongPressFired;                  // Gibt an, ob ein LongPressEvent seit dem letzten Loslassen ausgelöst wurde

  public:
    Event<> ActivatedEvent;               // Wird ausgelöst, wenn der Button gedrückt wird
    Event<> DeactivatedEvent;             // Wird ausgelöst, wenn der Button losgelassen wird
    Event<> ClickEvent;                   // Wird ausgelöst, wenn Drücken und Loslassen kurz hintereinander registriert werden
    Event<> LongPressEvent;               // Wird ausgelöst, wenn der Button über eine längere Zeit gedrückt gehalten wird

    // Konstruktor
  public:
    Button(const unsigned int clickTime = 500, const unsigned int longPressTime = 2000);

    // Methoden
  public:
    // Initialisiert den Button
    void Begin(Trigger* trigger);

    // Muss in jedem Frame aufgerufen werden
    void Update();

  private:
    // Wird aufgerufen, wenn der Trigger aktiviert wird
    void Activate();

    // Wird aufgerufen, wenn der Trigger deaktiviert wird
    void Deactivate();

    // Gibt an, ob der Trigger ausgelöst ist
    bool IsActive() const;
};

#endif
