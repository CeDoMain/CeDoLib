#ifndef ADAFRUITTFTDISPLAY_H
#define ADAFRUITTFTDISPLAY_H

#include "AdafruitTftElement.h"

template<typename T> class TftDisplay : public TftElement
{
    // Felder
public:    
    Delegate<void, Adafruit_GFX*, word, word, word, word, T>* ContentDrawer;    // Wird aufgerüfen, wenn der Inhalt neu gezeichnet werden soll (gfx, x, y, w, h, value)

private:
    T Value;                                                                    // Wert, der dargestellt werden soll

    // Konstruktor
public:
    TftDisplay(word left, word top, word width, word height, T value)
        : TftElement(left, top, width, height), ContentDrawer(0), Value(value)
    {
        
    }

    // Methoden
public:
    // Wird aufgerufen, wenn die Anzeige gedrückt wurde
    void Touch(word x, word y)
    {

    }

    // Stellt einen neuen Wert auf der Anzeige dar
    void SetValue(T value)
    {
        if (Value == value)
            return;
        
        Value = value;
        
        if (GetVisibility())
            Draw();
    }
    
private:
    // Zeichnet die Anzeige
    void Draw()
    {
        if (ContentDrawer != 0) (*ContentDrawer)(Gfx, Left, Top, Width, Height, Value);
    }

};

#endif
