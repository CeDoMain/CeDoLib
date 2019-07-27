#ifndef ADAFRUITTFTBUTTON_H
#define ADAFRUITTFTBUTTON_H

#include "AdafruitTftElement.h"
#include "Delegate.h"
#include "Timer.h"

class TftButton : public TftElement
{
    // Felder
public:
    Delegate<>* ClickEvent;                                             // Wird ausgelöst, wenn der Knopf gedrückt wurde
    Delegate<void, Adafruit_GFX*, word, word, bool>* ContentDrawer;     // Wird aufgerüfen, wenn der Inhalt neu gezeichnet werden soll (gfx, x, y, isPressed)
    
private:
    Timer ReleaseTimer;                                                 // Timer, um den Knopf zu deaktivieren

    // Konstruktor
public:
    TftButton(word left, word top, word width, word height);

    // Methoden
public:
    // Initialisiert den Knopf
    virtual void Begin();

    // Muss in jedem Frame aufgerufen werden
    virtual void Update();

    // Wird aufgerufen, wenn der Knopf gedrückt wurde
    virtual void Touch(word x, word y);

    // Zeichnet den Inhalt neu
    void DrawContent();

private:
    // Zeichnet den Knopf
    virtual void Draw();
    void Draw(bool isPressed);
};

#endif
