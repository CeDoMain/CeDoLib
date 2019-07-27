#ifndef ADAFRUITTFT35_H
#define ADAFRUITTFT35_H

#include "STMPE610.h"
#include "DS3231.h"
#include "Timer.h"
#include "Fader.h"
#include "AdafruitTftContainer.h"
#include <Adafruit_HX8357.h>

class AdafruitTft35
{
    // Felder
public:
    static const word Width = HX8357_TFTHEIGHT; // Breite des Bildschirms (Quer)
    static const word Height = HX8357_TFTWIDTH; // Höhe des Bildschirms (Quer)

private:
    Adafruit_HX8357 Tft;                        // Bildschirm
    STMPE610 Touchscreen;                       // Touchscreen
    bool KeysLocked;                            // Gibt an, ob der Touchscreen gedimmt/gesperrt ist
    Timer DisplayDimmer;                        // Timer, der das Display dimmt
    Fader DisplayFader;                         // Fader, der das Display fadet
    TftContainer TopContainer;                  // Oberstes Bedienelement

    // Konstruktor
public:
    AdafruitTft35(signed char tftCsPin, signed char tftDcPin, unsigned long fadeOutDelay = 25000);

    // Methoden
public:
    // Initialisiert den Touchscreen
    void Begin(AnalogOut* displayLightPin, DigitalOut* touchCsPin, unsigned long fadeOutTime = 5000, unsigned long fadeInTime = 1000);

    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Fügt dem Bildschirm ein Bedienelement hinzu
    void Add(TftElement* e);

};

#endif
