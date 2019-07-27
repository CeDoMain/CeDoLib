#ifndef ADAFRUITSELECTBOX_H
#define ADAFRUITSELECTBOX_H

#include "AdafruitTftElement.h"
#include "Delegate.h"

class TftSelectBox : public TftElement
{
    // Felder
public:
    Delegate<void, word>* ValueChangedEvent;                        // Wird ausgelöst, wenn sich der Wert ändert (neuer Wert)
    Delegate<void, Adafruit_GFX*, word, word, word>* ContentDrawer; // Wird aufgerüfen, wenn der Inhalt neu gezeichnet werden soll (gfx, x, y, Wert)

private:
    word Value;                                                     // Wert
    const word MinValue, MaxValue;                                  // Wertebereich
    const bool IsHorizontal;                                        // Gibt die Ausrichtung der Wippen an
    const byte RockerSize;                                          // Breite bzw. Höhe eines Wippbereichs (mit Rand)

    static const byte SymbolPadding = 2 * BorderThickness;          // Abstand zwischen Rand und Wippensymbol

    // Konstruktor
public:
    TftSelectBox(word left, word top, word width, word height, byte rockerSize,
        word minValue, word maxValue, bool isHorizontal = true);

    // Methoden
public:
    // Wird aufgerufen, wenn das Auswahlfeld gedrückt wurde
    virtual void Touch(word x, word y);

    // Verändert den Wert
    void SetValue(word value, bool raiseEvent = true);

    // Ließt den Wert
    word GetValue();
    
    // Zeichnet den Inhalt neu
    void DrawContent();

private:
    // Zeichnet das gesamte Auswahlfeld
    virtual void Draw();

    // Zeichnet das Textfeld
    void DrawTextBox();

};

#endif
