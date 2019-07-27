#ifndef ADAFRUITTFTTABCONTROL_H
#define ADAFRUITTFTTABCONTROL_H

#include "AdafruitTftElement.h"
#include "AdafruitTftContainer.h"
#include "LinkedList.h"
#include "Delegate.h"

class TftTabControl : public TftElement
{
    // Felder
private:
    byte SelectedTab;           // ausgewählte Seite
    const byte TabCount;        // Anzahl an Seiten
    const word RibbonWidth;     // Breite der Buttonspalte mit Rand
    const word ButtonHeight;    // Höhe eines Buttons (inkl. 1x Rand)
    TftContainer** Pages;       // Seiten
    Delegate<void, Adafruit_GFX*, word, word, bool>** ContentDrawer;    // Tab-Beschriftung
    
    // Konstruktor
public:
    TftTabControl(word left, word top, word width, word height, byte tabCount, word ribbonWidth);

    // Methoden
public:
    // Initialisiert das TabControl
    virtual void Begin();

    // Muss in jedem Frame aufgerufen werden
    virtual void Update();
    
    // Muss vom Elternelement einmal aufgerufen werden
    virtual void SetParent(Adafruit_GFX* gfx, word offsetX, word offsetY);

    // Wird aufgerufen, wenn das TabControl gedrückt wurde
    virtual void Touch(word x, word y);

    // Setzt eine neue Sichtbarkeit
    virtual void SetVisibility(bool value);

    // Wählt eine Seite aus
    void SelectTab(byte index);

    // Fügt ein Element der angegebenen Seite hinzu
    void Add(byte index, TftElement* e);

    // Ändert den Inhalt des angegebenen Knopfes
    void SetButtonContent(byte index, Delegate<void, Adafruit_GFX*, word, word, bool>* contentDrawer);

private:
    // Zeichnet das TabControl
    virtual void Draw();
};

#endif
