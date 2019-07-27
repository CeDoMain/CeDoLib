#ifndef ADAFRUITTFTCONTAINER_H
#define ADAFRUITTFTCONTAINER_H

#include "AdafruitTftElement.h"
#include "LinkedList.h"

class TftContainer : public TftElement
{
    // Felder
private:
    LinkedList<TftElement*> Elements;    // Alle enthaltenen Elemente

    // Konstruktor
public:
    TftContainer(word left, word top, word width, word height);

    // Methoden
public:
    // Initialisiert den Container
    virtual void Begin();

    // Muss in jedem Frame aufgerufen werden
    virtual void Update();

    // Wird aufgerufen, wenn der Container gedrückt wurde
    virtual void Touch(word x, word y);
    
    // Setzt eine neue Sichtbarkeit
    virtual void SetVisibility(bool value);

    // Fügt dem Container ein Element hinzu
    void Add(TftElement* e);

private:
    // Zeichnet den Container
    virtual void Draw();

};

#endif
