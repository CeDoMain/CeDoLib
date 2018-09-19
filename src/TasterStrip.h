#ifndef TASTERSTRIP_H
#define TASTERSTRIP_H

#include "PCA9685.h"
#include "MCP23017.h"
#include "BiColorLED.h"
#include "Button.h"
#include "Trigger.h"


class TasterStrip
{
    // Felder
public:
    BiColorLED Led[5];
    Button Btn[5];                       

private:
    PCA9685 PWMchip;
    MCP23017 GPIOchip;

    // Konstruktor
public:
    TasterStrip(byte A2A1A0);

    // Methoden
public:
    // Initialisiert den Strip
    void Begin();

    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Alle LEDs einschalten
    void AllLedOn();
};

#endif