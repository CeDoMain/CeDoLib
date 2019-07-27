#ifndef ADAFRUITTFTNUMERICUPDOWN_H
#define ADAFRUITTFTNUMERICUPDOWN_H

#include "AdafruitTftElement.h"
#include "Delegate.h"

class TftNumericUpDown : public TftElement
{
    // Felder
public:
    Delegate<void, word>* ValueChangedEvent;            // Wird ausgelöst, wenn sich der Wert ändert (neuer Wert)

private:
    word Value;                                         // Wert
    const word MinValue, MaxValue;                      // Wertebereich
    const byte DigitCount;                              // Anzahl der Ziffern
    const byte RockerCount;                             // Anzahl der Wippen (von rechts)
    const Color565 FontForeground, FontBackground;      // Farben des Schriftfeldes
    const GFXfont* FontType;                            // Schriftart der Ziffern

    const byte RockerWidth;                             // X-Raster der Wippensymbole (Symbolbreite + 1x Padding)
    const byte RockerHeight;                            // Höhe des Wippbereichs (mit 1x Rand)
    const byte DigitHeight;                             // Höhe des Schriftfeldes
    word FontPosY, FontPosX;                            // Absolute Cursorposition der linken Ziffer

    static const byte SymbolPadding = 2 * BorderThickness;          // Abstand zwischen Rand und Wippensymbol

    // Konstruktor
public:
    TftNumericUpDown(word left, word top, word width, word height, word minValue, word maxValue,
        Color565 fontForeground, Color565 fontBackground, const GFXfont* fontType);

    // Methoden
public:
    // Initialisiert das Nummernfeld
    virtual void Begin();

    // Wird aufgerufen, wenn das Nummernfeld gedrückt wurde
    virtual void Touch(word x, word y);

    // Verändert den Wert
    void SetValue(word value, bool raiseEvent = true);

    // Ließt den Wert
    word GetValue();

private:
    // Zeichnet das gesamte Nummernfeld
    virtual void Draw();

    // Zeichnet das Anzeigefeld
    void DrawTextBox();

};

#endif
