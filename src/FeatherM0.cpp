#include "FeatherM0.h"

#ifdef ARDUINO_SAMD_ZERO

DigitalOut* FeatherM0::GetDigitalOut(const DigitalPin pin)
{
    // Wrapper für einen Digitalausgang erzeugen
    pinMode(pin, OUTPUT);
    return new DigitalOut([pin](bool value) 
    {
        digitalWrite(pin, value ? HIGH : LOW);
    });
}
DigitalIn* FeatherM0::GetDigitalIn(const DigitalPin pin, const bool isPulledUp)
{
    // Wrapper für einen Digitaleingang erzeugen und Pullup konfigurieren
    pinMode(pin, isPulledUp ? INPUT_PULLUP : INPUT);
    return new DigitalIn([pin]() -> bool
    {
        return digitalRead(pin) == HIGH;
    });
}
AnalogOut* FeatherM0::GetAnalogOut(const AnalogOutPin pin)
{
    // Wrapper für einen Analogausgang erzeugen
    if (pin != AnalogOutPin::DAC0)
        pinMode(pin, OUTPUT);
    return new AnalogOut([pin](float value) 
    {
        return analogWrite(pin, (byte)(value * 255));
    });
}
AnalogIn* FeatherM0::GetAnalogIn(const AnalogInPin pin, AnalogReference reference)
{
    // Wrapper für einen Analogeingang erzeugen und Referenzspannung konfigurieren
    if (reference == AnalogReference::External)
        analogReference(AR_EXTERNAL);
    return new AnalogIn([pin]() -> float
    {
        return analogRead(pin) / 1023.0;
    });
}

#endif
