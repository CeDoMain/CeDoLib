#include "AdafruitTftRadioButton.h"

LinkedList<TftRadioButton*> TftRadioButton::Buttons = LinkedList<TftRadioButton*>();

TftRadioButton::TftRadioButton(word left, word top, word width, word height, byte groupId, bool canTouchDeactivate)
    : TftElement(left, top, width, height), ActivationChangedEvent(0), ContentDrawer(0),
      GroupId(groupId), CanTouchDeactivate(canTouchDeactivate), IsActive(false)
{
    Buttons.Add(this);
}
void TftRadioButton::Touch(word x, word y)
{
    SetActivation(IsActive ? !CanTouchDeactivate : true, true, true);
}
void TftRadioButton::SetActivation(bool value, bool raiseEvent, bool wasTouched)
{
    if (value == IsActive)
        return;

    IsActive = value;
    
    if (GetVisibility())
        Draw();

    // Andere Auswahlknöpfe der gleichen Gruppe deaktivieren
    if (IsActive)
    {
        Buttons.ForEach([=](TftRadioButton* btn)
        {
            if (btn != this && btn->GroupId == GroupId)
                btn->SetActivation(false, raiseEvent, false);
        });
    }

    if (!raiseEvent)
        return;
        
    if (ActivationChangedEvent != 0)
        (*ActivationChangedEvent)(IsActive, wasTouched);

}
void TftRadioButton::DrawContent()
{
    if (ContentDrawer != 0)
        (*ContentDrawer)(Gfx, Left + Width / 2, Top + Height / 2, IsActive);
}
void TftRadioButton::Draw()
{
    DrawRect(Left, Top, Width, Height,
        IsActive ? TouchForeground : ControlForeground, IsActive ? TouchBackground : ControlBackground);
    if (ContentDrawer != 0)
        (*ContentDrawer)(Gfx, Left + Width / 2, Top + Height / 2, IsActive);
}