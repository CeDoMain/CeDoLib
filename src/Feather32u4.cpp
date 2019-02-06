#include "Feather32u4.h"

#ifdef ARDUINO_AVR_FEATHER32U4

DigitalOut* Feather32u4::GetDigitalOut(const DigitalPin pin)
{
    // Wrapper für einen Digitalausgang erzeugen
    pinMode(pin, OUTPUT);
    return new DigitalOut([pin](const bool value) 
    {
        digitalWrite(pin, value ? HIGH : LOW);
    });
}
DigitalIn* Feather32u4::GetDigitalIn(const DigitalPin pin, const bool isPulledUp)
{
    // Wrapper für einen Digitaleingang erzeugen und Pullup konfigurieren
    pinMode(pin, isPulledUp ? INPUT_PULLUP : INPUT);
    return new DigitalIn([pin, isPulledUp]() -> bool
    {
        return digitalRead(pin) == HIGH;
    });
}
AnalogOut* Feather32u4::GetAnalogOut(const AnalogOutPin pin)
{
    // Wrapper für einen Analogausgang erzeugen
    pinMode(pin, OUTPUT);
    return new AnalogOut([pin](const float value) 
    {
        return analogWrite(pin, (byte)(value * 255));
    });
}
AnalogIn* Feather32u4::GetAnalogIn(const AnalogInPin pin, AnalogReference reference)
{
    // Wrapper für einen Analogeingang erzeugen und Referenzspannung konfigurieren
    analogReference(reference);
    return new AnalogIn([pin]() -> bool
    {
        return analogRead(pin / 1023.0);
    });
}

#endif
