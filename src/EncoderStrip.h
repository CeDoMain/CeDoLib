#ifndef ENCODERSTRIP_H
#define ENCODERSTRIP_H

#include "Button.h"
#include "SSD1331.h"
#include "MCP23017.h"

class EncoderStrip
{
    // Felder
public:
    SSD1331 Disp;                                   // Display des Encoderstrips
    Button ButtonTop, ButtonBtm;                    // Buttons der Encoder
    Delegate<void, short>* EncoderTopChangedEvent;  // Wird ausgelöst, wenn der obere Encoder gedreht wurde
    Delegate<void, short>* EncoderBtmChangedEvent;  // Wird ausgelöst, wenn der untere Encoder gedreht wurde

private:
    static const byte AtTinyBaseAddress = 0x78;     // Adressprefix des AtTiny
    const byte I2CAddressAtTiny;                    // mit dem Prefix kombinierte Busadresse
    MCP23017 Gpio;                                  // GPIO Chip des Strips um das Display zu adressieren
    bool ButtonTopState, ButtonBtmState;            // aktueller Zustand der Encodertaster (Hilfsvariable)

    // Konstruktor
public:
    EncoderStrip(byte A2A1A0);

    // Methoden
public:
    // Initialisiert den Strip
    void Begin();

    // Muss in jedem Frame aufgerufen werden
    void Update();
};

#endif