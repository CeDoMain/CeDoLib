#ifndef MCP23017_H
#define MCP23017_H

#include "Arduino.h"
#include "Wire.h"
#include "BaseIO.h"

class MCP23017
{
    // Aufzählungen
public:
    enum IOPort
    {
        GPA7 = 15,
        GPA6 = 14,
        GPA5 = 13,
        GPA4 = 12,
        GPA3 = 11,
        GPA2 = 10,
        GPA1 = 9,
        GPA0 = 8,
        GPB7 = 7,
        GPB6 = 6,
        GPB5 = 5,
        GPB4 = 4,
        GPB3 = 3,
        GPB2 = 2,
        GPB1 = 1,
        GPB0 = 0
    };
    // Felder
private:
    static const byte REG_DIRECTION = 0x00;     // Register für die Richtungskonfiguration
    static const byte REG_PULLUP = 0x0C;        // Register für die Pullupwiderstände
    static const byte REG_INPUT = 0x12;         // Register für den GPIO Portwert
    static const byte REG_OUTPUT = 0x14;        // Register für die Outputlatches
    static const byte BASE_ADDRESS = 0x20;      // Basisadresse des Chips

    const byte I2CAddress;      // 7bit Adresse des Chips im I2C-Bus
    word Direction;             // Konfiguration als Eingang (1) oder Ausgang (0) jedes Ports
    word Pullup;                // Aktivierung (1) oder Deaktivierung (0) des Pullups jedes Ports
    word Input;                 // Wert jedes Eingangs (High 1, Low 0)
    word Output;                // Wert jedes Ausgangs (High 1, Low 0)

    // Konstruktor
public:
    MCP23017(const byte A2A1A0);

    // Methoden
public:
    // Initialisiert den Chip
    void Begin();

    // Muss in jedem Frame aufgerufen werden, wenn Eingänge abgefragt werden sollen (Cache update)
    void Update();

    // Ließt den Wert eines Eingangs aus dem Cache
    DigitalIn* GetDigitalIn(const IOPort pin, const bool isPulledUp = true);

    // Ändert den Wert eines Ausgangs
    DigitalOut* GetDigitalOut(const IOPort pin);

    // Pins konfigurieren
    void PinMode(const IOPort pin, const int mode);

private:
    // Schreibt die Daten in das angegebene Register
  	void WriteWordRegister(const byte regAddress, const word data);
    void WriteByteRegister(const byte regAddress, const byte data);

    // Liest das Word aus dem angegebenen Register
  	word ReadWordRegister(const byte regAddress);
};

#endif
