#ifndef ADAFRUITTFTRADIOBUTTON_H
#define ADAFRUITTFTRADIOBUTTON_H

#include "AdafruitTftElement.h"
#include "Delegate.h"
#include "LinkedList.h"

class TftRadioButton : public TftElement
{
    // Felder
public:
    Delegate<void, bool, bool>* ActivationChangedEvent;                                      // Wird ausgelöst, wenn der Auswahlknopf aktiviert wurde (isActive, wasTouched)
    Delegate<void, Adafruit_GFX*, word, word, bool>* ContentDrawer;                     // Wird aufgerüfen, wenn der Inhalt neu gezeichnet werden soll (gfx, x, y, isActive)

private:
    const byte GroupId;                                                                 // Gibt an, in welcher Gruppe der Auswahlknopf ist
    const bool CanTouchDeactivate;                                                      // Gibt an, ob der Auswahlknopf durch drücken deaktiviert werden kann
    bool IsActive;                                                                      // Gibt an, ob die Option ausgewählt ist
    static LinkedList<TftRadioButton*> Buttons;                                         // Alle vorhandenen Auswahlknöpfe

    // Konstruktor
public:
    TftRadioButton(word left, word top, word width, word height, byte groupId, bool canTouchDeactivate = false);

    // Methoden
public:
    // Wird aufgerufen, wenn der Auswahlknopf gedrückt wurde
    virtual void Touch(word x, word y);

    // Ändert den Aktivierungszustand
    void SetActivation(bool value, bool raiseEvent = true, bool wasTouched = false);
    
    // Zeichnet den Inhalt neu
    void DrawContent();

private:
    // Zeichnet den Auswahlknopf
    virtual void Draw();

};

#endif
