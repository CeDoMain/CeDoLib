#include "AdafruitTftRadioButton.h"

LinkedList<TftRadioButton*> TftRadioButton::Buttons = LinkedList<TftRadioButton*>();

TftRadioButton::TftRadioButton(word left, word top, word width, word height, byte groupId, bool canTouchDeactivate)
    : TftElement(left, top, width, height), ActivatedEvent(0), DeactivatedEvent(0), ContentDrawer(0),
      GroupId(groupId), CanTouchDeactivate(canTouchDeactivate), IsActive(false)
{
    Buttons.Add(this);
}
void TftRadioButton::Touch(word x, word y)
{
    SetActivation(IsActive ? !CanTouchDeactivate : true);
}
void TftRadioButton::SetActivation(bool value, bool raiseEvent)
{
    if (value == IsActive)
        return;

    IsActive = value;
    
    Draw();

    // Andere Auswahlknöpfe der gleichen Gruppe deaktivieren
    if (IsActive)
    {
        Buttons.ForEach([=](TftRadioButton* btn)
        {
            if (btn != this && btn->GroupId == GroupId)
                btn->SetActivation(false, raiseEvent);
        });
    }

    if (raiseEvent)
        return;
        
    if (!IsActive && DeactivatedEvent != 0)
        (*DeactivatedEvent)();
    if (IsActive && ActivatedEvent != 0)
        (*ActivatedEvent)();

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