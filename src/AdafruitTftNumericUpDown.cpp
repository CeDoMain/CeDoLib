#include "AdafruitTftNumericUpDown.h"

TftNumericUpDown::TftNumericUpDown(word left, word top, word width, word height, word minValue, word maxValue,
        Color565 fontForeground, Color565 fontBackground, const GFXfont* fontType)
    : TftElement(left, top, width, height), ValueChangedEvent(0), Value(minValue), MinValue(minValue), MaxValue(maxValue),
      DigitCount(ceilf(log10f(maxValue))), FontForeground(fontForeground), FontBackground(fontBackground), FontType(fontType),
      // Wenn die höchste Stelle weniger als 2 verschiedene Ziffern hat, bekommt sie keine Wippe
      RockerCount(ceilf(log10f(MaxValue - MinValue)) - ((MaxValue - MinValue / powf(10, ceilf(log10f(MaxValue - MinValue))) < 2) ? 1 : 0)),
      RockerWidth((Width - 2 * BorderThickness - SymbolPadding) / DigitCount), RockerHeight(RockerWidth + SymbolPadding + BorderThickness),
      DigitHeight(Height - 2 * RockerHeight), FontPosX(0), FontPosY(0)
{

}
void TftNumericUpDown::Begin()
{
    TftElement::Begin();

    // Absolute Cursorposition der linken Ziffer berechnen
    Gfx->setFont(FontType);
    Gfx->setTextSize(1);
    short a, b;
    word c, d;
    Gfx->getTextBounds("8", 0, 0, &a, &b, &c, &d);
    FontPosX = Left + BorderThickness + (SymbolPadding + RockerWidth - c - a) / 2;
    FontPosY = Top + RockerHeight + (DigitHeight - b) / 2;
}
void TftNumericUpDown::Touch(word x, word y)
{
    y -= Top;

    // Prüfen ob eine Wippe berührt wurde
    if (RockerHeight < y && y < RockerHeight + DigitHeight)
        return;

    int newValue = Value;

    x -= Left;
    word xThresh = Width - BorderThickness - RockerWidth - SymbolPadding / 2;
    for (int i = 1; i <= RockerCount; i++)
    {
        // Stelle der Berührung finden
        if (xThresh <= x)
        {
            if (y <= RockerHeight)
            {
                // Ziffer erhöhen
                if (newValue == MaxValue)
                {
                    newValue = MinValue;
                }
                else
                {
                    newValue += powf(10, i - 1);
                    if (newValue > MaxValue)
                        newValue = MaxValue;
                }
            }
            else
            {
                // Ziffer verringern
                if (newValue == MinValue)
                {
                    newValue = MaxValue;
                }
                else
                {
                    newValue -= powf(10, i - 1);
                    if (newValue < MinValue)
                        newValue = MinValue;
                }
            }
            SetValue(newValue);
            return;
        }
        xThresh -= RockerWidth;
    }
}
void TftNumericUpDown::SetValue(word value, bool raiseEvent)
{
    if (Value == value)
        return;
        
    Value = max(MinValue, min(value, MaxValue));

    if (GetVisibility())
        DrawTextBox();

    if (raiseEvent && ValueChangedEvent != 0)
        (*ValueChangedEvent)(Value);
}
word TftNumericUpDown::GetValue()
{
    return Value;
}
void TftNumericUpDown::Draw()
{
    // Umrandung
    DrawRect(Left, Top, Width, RockerHeight + BorderThickness, ContainerForeground, ContainerBackground);
    DrawRect(Left, Bottom + 1 - RockerHeight - BorderThickness, Width, RockerHeight + BorderThickness, ContainerForeground, ContainerBackground);
    
    // Schriftfeld
    DrawTextBox();

    word leftX = Right + 1 - BorderThickness - RockerWidth;
    const word triangleSize = RockerWidth - SymbolPadding;
    for (int i = 0; i < RockerCount; i++)
    {
        // Wippen
        DrawTriangle(leftX, Top + BorderThickness + SymbolPadding, triangleSize, triangleSize, 0, ControlForeground, ControlBackground);
        DrawTriangle(leftX, Bottom + 1 - BorderThickness - SymbolPadding - triangleSize, triangleSize, triangleSize, 2, ControlForeground, ControlBackground);
        leftX -= RockerWidth;
    }
}
void TftNumericUpDown::DrawTextBox()
{
    // Schriftfeld
    DrawRect(Left, Top + RockerHeight, Width, DigitHeight, FontForeground, FontBackground);

    Gfx->setFont(FontType);
    Gfx->setTextColor(FontForeground);
    Gfx->setTextSize(1);

    char text[DigitCount + 1];
    sprintf(text, "%0*hu", DigitCount, Value);
    word leftX = FontPosX;
    for (int i = 0; i < DigitCount; i++)
    {
        // Ziffern
        Gfx->setCursor(leftX, FontPosY);
        Gfx->print(text[i]);
        leftX += RockerWidth;
    }
}