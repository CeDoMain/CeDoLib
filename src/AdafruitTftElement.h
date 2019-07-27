#ifndef ADAFRUITTFTELEMENT_H
#define ADAFRUITTFTELEMENT_H

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include "Debug.h"

typedef word Color565;
#define C565(r,g,b) (((r & 0xF8) << 8) + ((g & 0xFC) << 3) + ((b & 0xF8) >> 3))

class TftElement
{
    // Felder
public:
    word Left, Right, Top, Bottom;              // Ränder des Elements
    const word Width, Height;                   // Dimensionen des Elements

    static const Color565 ContainerBackground = C565(0x3F, 0x3F, 0x3F);
    static const Color565 ContainerForeground = C565(0x97, 0x97, 0x97);
    static const Color565 ControlBackground =   C565(0x00, 0x00, 0x00);
    static const Color565 ControlForeground =   C565(0xFF, 0xFF, 0xFF);
    static const Color565 TouchBackground =     C565(0x00, 0x00, 0x00);
    static const Color565 TouchForeground =     C565(0xFF, 0x0F, 0x00);

    static const byte CornerRadius = 8;         // Muss größer gleich der Randdicke sein!
    static const byte BorderThickness = 4;      // Randdicke

protected:
    Adafruit_GFX* Gfx;                          // Zeichenfläche

private:
    bool IsVisible;                             // Gibt an, ob das Element sichtbar ist

    // Konstruktor
public:
    TftElement(word left, word top, word width, word height);

    // Methoden
public:
    // Initialisiert das Element
    virtual void Begin();

    // Muss in jedem Frame aufgerufen werden
    virtual void Update();

    // Muss vom Elternelement einmal aufgerufen werden
    virtual void SetParent(Adafruit_GFX* gfx, word offsetX, word offsetY);

    // Wird aufgerufen, wenn das Element gedrückt wurde (Bereich und Sichtbarkeit vor dem Aufruf prüfen)
    virtual void Touch(word x, word y) = 0;

    // Setzt eine neue Sichtbarkeit
    virtual void SetVisibility(bool value);

    // Gibt an, ob das Element sichtbar ist
    bool GetVisibility();

protected:
    // Zeichnet das Element neu
    virtual void Draw() = 0;

    // Zeichnet umrandete Formen
    void DrawTriangle(word l, word t, word w, word h, byte r, Color565 f, Color565 b);
    void DrawRect(word l, word t, word w, word h, Color565 f, Color565 b);

};

#endif