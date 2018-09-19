#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Event.h"
#include "Trigger.h"

class Button
{
    // Felder
public:
    Delegate<>* ActivatedEvent;           // Wird ausgelöst, wenn der Button gedrückt wird
    Delegate<>* DeactivatedEvent;         // Wird ausgelöst, wenn der Button losgelassen wird
    Delegate<>* ClickEvent;               // Wird ausgelöst, wenn Drücken und Loslassen kurz hintereinander registriert werden
    Delegate<>* LongPressEvent;           // Wird ausgelöst, wenn der Button über eine längere Zeit gedrückt gehalten wird

private:
    Trigger* Trig;                        // Trigger, der den Button auslöst
    const unsigned int ClickTime;         // Zeit in ms, die maximal zwischen Drücken und Loslassen vergehen darf, um einen Click auszulösen
    const unsigned int LongPressTime;     // Zeit in ms, die der Button für einen langen Druck minimal gedrückt sein muss
    unsigned long ActivatedTimeStamp;     // Zeit in ms, zu der der Button das letzte Mal gedrückt wurde
    unsigned long DeactivatedTimeStamp;   // Zeit in ms, zu der der Button das letzte Mal losgelassen wurde
    bool LongPressFired;                  // Gibt an, ob ein LongPressEvent seit dem letzten Loslassen ausgelöst wurde

    // Konstruktor
public:
    Button(const unsigned int clickTime = 500, const unsigned int longPressTime = 2000);

    // Methoden
public:
    // Initialisiert den Button
    void Begin(Trigger* trigger);

    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Gibt an, ob der Trigger ausgelöst ist
    bool IsActive() const;

private:
    // Wird aufgerufen, wenn der Trigger aktiviert wird
    void Activate();

    // Wird aufgerufen, wenn der Trigger deaktiviert wird
    void Deactivate();
};

#endif
