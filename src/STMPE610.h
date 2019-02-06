#ifndef STMPE610_H
#define STMPE610_H

#include "Arduino.h"
#include "SPI.h"
#include "BaseIO.h"

class STMPE610
{
    // Felder
public:
    Delegate<void, unsigned short, unsigned short>* TouchEvent;       // Wird ausgelöst, wenn der Touchscreen berührt wurde (X, Y)

private:
    static const byte REG_SYS_CTRL1 =           0x03;
    static const byte REG_SYS_CTRL2 =           0x04;
    static const byte REG_SPI_CFG =             0x08;
    static const byte REG_ADC_CTRL1 =           0x20;
    static const byte REG_ADC_CTRL2 =           0x21;
    static const byte REG_TSC_CTRL =            0x40;
    static const byte BIT_TSC_STA =             7;
    static const byte REG_TSC_CFG =             0x41;
    static const byte REG_WDW_TR_X =            0x42;
    static const byte REG_WDW_TR_Y =            0x44;
    static const byte REG_WDW_BL_X =            0x46;
    static const byte REG_WDW_BL_Y =            0x48;
    static const byte REG_FIFO_CTRL_STA =       0x4B;
    static const byte BIT_FIFO_EMPTY =          5;
    static const byte REG_TSC_FRACTION_Z =      0x56;
    static const byte REG_TSC_DATA =            0xD7;
    static const byte REG_TSC_I_DRIVE =         0x58;
    
    DigitalOut* Pin_CS;             // ChipSelect Pin
    bool WasTouched;                // Gibt an, ob der Touchscreen beim letzten Update gedrückt war
    byte Rotation;                  // Ausrichtung des Touchscreens
    unsigned long ReleaseTimeStamp; // Zeit, beim Loslassen

    // Konstruktor
public:
    STMPE610();

    // Methoden
public:
    // Initialisiert den Chip
    void Begin(DigitalOut* pin_CS);

    // Muss in jedem Frame aufgerufen werden, um das Empfangen zu ermöglichen
    void Update();

    // Konfiguriert Ausrichtung [0;4], 0-3 = TFT, 4 = Rohdaten
    void SetRotation(byte r);

private:
    // Bussteuerung
    void SpiBegin();
    void SpiEnd();

    // Sendet Daten zum angegebenen Register
    void WriteRegByte(byte regAddress, byte newByte);
    void WriteRegWord(byte regAddress, word newWord);

    // Ließt die Daten aus dem angegebenen Register
    byte ReadRegByte(byte regAddress);

    // Ließt den ersten Wert aus dem FiFo
    void ReadFifo(unsigned short* x, unsigned short* y);

    // Leert das FiFo
    void ClearFifo();
};

#endif
