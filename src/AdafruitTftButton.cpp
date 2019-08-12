#include "AdafruitTftButton.h"

TftButton::TftButton(word left, word top, word width, word height)
    : TftElement(left, top, width, height), ClickEvent(0), ContentDrawer(0), ReleaseTimer(500)
{
    
}
void TftButton::Begin()
{
    ReleaseTimer.TimeIsUpEvent = new Delegate<>([=]()
    {
        Draw(false);
    });
}
void TftButton::Update()
{
    ReleaseTimer.Update();
}
void TftButton::Touch(word x, word y)
{
    Draw(true);
    ReleaseTimer.Start();
    if (ClickEvent != 0)
        (*ClickEvent)();
}
void TftButton::DrawContent()
{
    if (ContentDrawer != 0)
        (*ContentDrawer)(Gfx, Left + Width / 2, Top + Height / 2, false);
}
void TftButton::Draw()
{
    Draw(false);
}
void TftButton::Draw(bool isPressed)
{
    DrawRect(Left, Top, Width, Height,
        isPressed ? TouchForeground : ControlForeground, isPressed ? TouchBackground : ControlBackground);
    if (ContentDrawer != 0)
        (*ContentDrawer)(Gfx, Left + Width / 2, Top + Height / 2, isPressed);
}