#include "Button.h"

Button::Button(const unsigned int clickTime, const unsigned int longPressTime)
  : Trig(0), ClickTime(clickTime), LongPressTime(longPressTime),
    ActivatedTimeStamp(0), DeactivatedTimeStamp(0), LastClickTimeStamp(0),
    LongPressFired(false), ActivatedEvent(), DeactivatedEvent(),
    ClickEvent(), LongPressEvent()
{
  
}

void Button::Begin(Trigger* trigger)
{
  Trig = trigger;
  Trig->Activated = Delegate<>(this, &Button::Activate);
  Trig->Deactivated = Delegate<>(this, &Button::Deactivate);
}

void Button::Activate()
{
  ActivatedTimeStamp = millis();

  ActivatedEvent();
}

void Button::Deactivate()
{
  DeactivatedTimeStamp = millis();
  LongPressFired = false;

  DeactivatedEvent();

  if (DeactivatedTimeStamp - ActivatedTimeStamp <= (unsigned long)ClickTime)
  {
    // gültiger Klick registriert
    ClickEvent();

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
    LongPressEvent();
  }
}

bool Button::IsActive() const
{
  return ActivatedTimeStamp > DeactivatedTimeStamp;
}
