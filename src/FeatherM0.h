#ifndef FEATHERM0_H
#define FEATHERM0_H

#ifdef ARDUINO_SAMD_ZERO

#include "BaseIO.h"
#include "Arduino.h"

class FeatherM0
{
    // Aufzählungen
public:
    enum DigitalPin
    {
        D0, D1,                                 // Serielle Ports
        D3 = 3, D4,                             // Buildin Shield Ports
        D5, D6,                                 // Digital Ports
        D8 = 8,                                 // Buildin Shield Ports
        D9, D10, D11, D12, D13, D14,            // Digital Ports
        D15, D16, D17, D18, D19,                // Digital Ports
        D20, D21,                               // I2C Ports
        D22, D23, D24                           // SPI Ports
    };
    enum AnalogOutPin
    {
        DAC0,                                    // Analogausgänge
        PWM5 = 5, PWM6, PWM9 = 9,
        PWM10, PWM11, PWM12, PWM13,
    };
    enum AnalogInPin
    {
        A0, A1, A2, A3, A4, A5,                 // Analogeingänge
        A7 = 7,
    };
    enum AnalogReference
    {
        Default = DEFAULT,
        External
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