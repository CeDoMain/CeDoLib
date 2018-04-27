#include "Button.h"

Button::Button(const unsigned int clickTime, const unsigned int longPressTime)
    : Trig(0), ClickTime(clickTime), LongPressTime(longPressTime),
      ActivatedTimeStamp(0), DeactivatedTimeStamp(0),
      LongPressFired(false), ActivatedEvent(0), DeactivatedEvent(0),
      ClickEvent(0), LongPressEvent(0)
{
  
}

void Button::Begin(Trigger* trigger)
{
    // Trigger mit dem Button verknüpfen
    Trig = trigger;
    Trig->ActivatedEvent = new Delegate<>(this, &Button::Activate);
    Trig->DeactivatedEvent = new Delegate<>(this, &Button::Deactivate);
}

void Button::Activate()
{
    // Trigger wurde aktiviert, Zeitpunkt speichern
    ActivatedTimeStamp = millis();

    // Wenn das Event verknüpft ist, Aktivierungsevent auslösen
    if (ActivatedEvent != 0)
        (*ActivatedEvent)();
}

void Button::Deactivate()
{
    // Trigger wurde deaktiviert, Zeitpunkt speichern
    DeactivatedTimeStamp = millis();

    // Langen Tastendruck zurücksetzen für die erneute Auslösung beim nächsten Druck
    LongPressFired = false;

    // Wenn das Event verknüpft ist, Deaktivierungsevent auslösen
    if (DeactivatedEvent != 0)
        (*DeactivatedEvent)();

    // Länge der Auslösung berechnen und Testen ob sie kurz genug für einen Klick war
    if (DeactivatedTimeStamp - ActivatedTimeStamp <= (unsigned long)ClickTime)
    {
        // gültiger Klick registriert, wenn das Event verknüpft ist, Klickevent auslösen
        if (ClickEvent != 0)
            (*ClickEvent)();
    }
}

void Button::Update()
{
    // Zugrundeliegenden Trigger updaten
    Trig->Update();

    // Auf gedrückt halten prüfen, wenn ein langer druck noch nicht ausgelöst wurde
    if (IsActive() && !LongPressFired && millis() - ActivatedTimeStamp >= (unsigned long)LongPressTime)
    {
        // Trigger wurde lange genug ausgelöst
        LongPressFired = true;

        // Wenn das Event verknüpft ist, Langdruckevent auslösen
        if (LongPressEvent != 0)
            (*LongPressEvent)();
    }
}

bool Button::IsActive() const
{
    // Gibt an, ob der Trigger aktiviert ist
    return ActivatedTimeStamp > DeactivatedTimeStamp;
}
