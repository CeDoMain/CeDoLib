#ifndef PCA9685_H
#define PCA9685_H

#include "Arduino.h"
#include "Wire.h"
#include "BaseIO.h"

typedef short decimal;

class PCA9685
{
    // Aufzählungen
public:
    enum LED
    {
      ALL = -1,
      LED0 = 0,
      LED1 = 1,
      LED2 = 2,
      LED3 = 3,
      LED4 = 4,
      LED5 = 5,
      LED6 = 6,
      LED7 = 7,
      LED8 = 8,
      LED9 = 9,
      LED10 = 10,
      LED11 = 11,
      LED12 = 12,
      LED13 = 13,
      LED14 = 14,
      LED15 = 15
    };
    // Felder
private:
    static const byte REG_MODE1 = 0x00;         // Register 1 für die Konfiguration
    static const byte REG_MODE2 = 0x01;         // Register 2 für die Konfiguration
    static const byte REG_PRESCALE = 0xFE;      // Register für die PWM Geschwindigkeit
    static const byte REG_LED0_ON = 0x06;       // Register für die ON-Zeit von LED0
    static const byte REG_ALL_LED_ON = 0xFA;    // Register für die ON-Zeit von allen LEDs
    static const byte BASE_ADDRESS = 0x40;      // Basisadresse des Chips

    const byte I2CAddress;                      // 7bit Adresse des Chips im I2C-Bus

    // Konstruktor
public:
    PCA9685(const byte A5A4A3A2A1A0);

    // Methoden
public:
    // PWM Frequenz von 24Hz (0) bis 1526Hz (1)
    void Begin(const decimal preScale = 10000);

    // Pins konfigurieren
    DigitalOut* GetDigitalOut(const LED led);
    AnalogOut* GetAnalogOut(const LED led);

    // LED dimmen (Wert von 0 bis 1)
    void AnalogWrite(const LED led, const float value);

private:
    // Schreibt die Daten in das angegebene Register
    void WriteByteRegister(const byte regAddress, const byte data);
};
#endif
