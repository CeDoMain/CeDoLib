#include "AdafruitTftSelectBox.h"

TftSelectBox::TftSelectBox(word left, word top, word width, word height, byte rockerSize,
    word minValue, word maxValue, bool isHorizontal)
    : TftElement(left, top, width, height), ValueChangedEvent(0), ContentDrawer(0), Value(minValue),
      MinValue(minValue), MaxValue(maxValue), IsHorizontal(isHorizontal), RockerSize(rockerSize)
{
    
}
void TftSelectBox::Touch(word x, word y)
{
    int newValue = Value;
    if (IsHorizontal ? x <= Left + RockerSize : Bottom - RockerSize <= y)
    {
        // Wert verringern
        newValue--;
        if (newValue < MinValue)
            newValue = MaxValue;
    }
    else if (IsHorizontal ? Right - RockerSize <= x : y <= Top + RockerSize)
    {
        // Wert erhöhen
        newValue++;
        if (newValue > MaxValue)
            newValue = MinValue;
    }
    else
    {
        // Anzeigefeld wurde berührt
        return;
    }
    SetValue(newValue);
}
void TftSelectBox::SetValue(word value, bool raiseEvent)
{
    if (Value == value)
        return;

    Value = max(MinValue, min(value, MaxValue));

    if (GetVisibility())
        DrawTextBox();

    if (raiseEvent && ValueChangedEvent != 0)
        (*ValueChangedEvent)(Value);
}
word TftSelectBox::GetValue()
{
    return Value;
}
void TftSelectBox::DrawContent()
{
    if (ContentDrawer != 0)
        (*ContentDrawer)(Gfx, Left + Width / 2, Top + Height / 2, Value);
}
void TftSelectBox::Draw()
{
    // Umrandung
    DrawRect(Left, Top, Width, Height, ContainerForeground, ContainerBackground);
    
    // Schriftfeld
    DrawTextBox();
    
    byte triangleSize = RockerSize - BorderThickness - 2 * SymbolPadding;
    byte borderPlusPadding = BorderThickness + SymbolPadding;
    // Wippen
    if (IsHorizontal)
    {
        // Wippen links und rechts zeichnen
        DrawTriangle(Left + borderPlusPadding, Top + borderPlusPadding,
            triangleSize, Height - 2 * borderPlusPadding, 3, ControlForeground, ControlBackground);
        DrawTriangle(Right + 1 - RockerSize + SymbolPadding, Top + borderPlusPadding,
            triangleSize, Height - 2 * borderPlusPadding, 1, ControlForeground, ControlBackground);
    }
    else
    {
        // Wippen oben und unten zeichnen
        DrawTriangle(Left + borderPlusPadding, Top + borderPlusPadding,
            Width - 2 * borderPlusPadding, triangleSize, 0, ControlForeground, ControlBackground);
        DrawTriangle(Left + borderPlusPadding, Bottom + 1 - RockerSize + SymbolPadding,
            Width - 2 * borderPlusPadding, triangleSize, 2, ControlForeground, ControlBackground);
    }
}
void TftSelectBox::DrawTextBox()
{
    if (IsHorizontal)
        DrawRect(Left + RockerSize, Top, Width - 2 * RockerSize, Height, ControlForeground, ControlBackground);
    else
        DrawRect(Left, Top + RockerSize, Width, Height - 2 * RockerSize, ControlForeground, ControlBackground);
        
    if (ContentDrawer != 0)
        (*ContentDrawer)(Gfx, Left + Width / 2, Top + Height / 2, Value);
}