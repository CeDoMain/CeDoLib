#ifndef FEATHER32U4_H
#define FEATHER32U4_H

#ifdef ARDUINO_AVR_FEATHER32U4

#include "BaseIO.h"
#include "Arduino.h"

class Feather32u4
{
    // Aufzählungen
public:
    enum DigitalPin
    {
        D0, D1,                                 // Serielle Ports
        D2, D3,                                 // I2C Ports
        D4,                                     // Buildin Shield Ports
        D5, D6,                                 // Digital Ports
        D7, D8,                                 // Buildin Shield Ports
        D9, D10, D11, D12, D13,                 // Digital Ports
        D14, D15, D16,                          // SPI Ports
        D18 = 18, D19, D20, D21, D22, D32       // Digital Ports
    };
    enum AnalogOutPin
    {
        PWM3 = 3, PWM5 = 5, PWM6, PWM9 = 9,     // Analogausgänge
        PWM10, PWM11, PWM13 = 13
    };
    enum AnalogInPin
    {
        A0, A1, A2, A3, A4, A5,                 // Analogeingänge
        A7 = 7, A9 = 9, A10, A11
    };
    enum AnalogReference
    {
        Default = DEFAULT,
        External = EXTERNAL
    };

    // Methoden
public:
    // Erzeugt einen Wrapper für einen Digitalausgang
    static DigitalOut* GetDigitalOut(const DigitalPin pin);

    // Erzeugt einen Wrapper für einen Digitaleingang
    static DigitalIn* GetDigitalIn(const DigitalPin pin, const bool isPulledUp = true);

    // Erzeugt einen Wrapper für einen Analogausgang
    static AnalogOut* GetAnalogOut(const AnalogOutPin pin);

    // Erzeugt einen Wrapper für einen Analogeingang
    static AnalogIn* GetAnalogIn(const AnalogInPin pin, AnalogReference reference = AnalogReference::Default);
};

#endif

#endif