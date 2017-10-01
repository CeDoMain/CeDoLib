#include "Button.h"

Button::Button(const unsigned int clickTime, const unsigned int longPressTime)
    : Trig(0), ClickTime(clickTime), LongPressTime(longPressTime),
      ActivatedTimeStamp(0), DeactivatedTimeStamp(0), LastClickTimeStamp(0),
      LongPressFired(false), ActivatedEvent(0), DeactivatedEvent(0),
      ClickEvent(0), LongPressEvent(0)
{
  
}

void Button::Begin(Trigger* trigger)
{
    Trig = trigger;
    Trig->ActivatedEvent = new Delegate<>(this, &Button::Activate);
    Trig->DeactivatedEvent = new Delegate<>(this, &Button::Deactivate);
}

void Button::Activate()
{
    ActivatedTimeStamp = millis();
    if (ActivatedEvent != 0)
        (*ActivatedEvent)();
}

void Button::Deactivate()
{
    DeactivatedTimeStamp = millis();
    LongPressFired = false;

    if (DeactivatedEvent != 0)
        (*DeactivatedEvent)();

    if (DeactivatedTimeStamp - ActivatedTimeStamp <= (unsigned long)ClickTime)
    {
        // gültiger Klick registriert
        if (ClickEvent != 0)
            (*ClickEvent)();

        LastClickTimeStamp = DeactivatedTimeStamp;
    }
}

void Button::Update()
{
  Trig->Update();

  // Auf gedrückt halten prüfen
  if (IsActive() && !LongPressFired && millis() - ActivatedTimeStamp >= (unsigned long)LongPressTime)
  {
      LongPressFired = true;
      if (LongPressEvent != 0)
          (*LongPressEvent)();
  }
}

bool Button::IsActive() const
{
    return ActivatedTimeStamp > DeactivatedTimeStamp;
}
