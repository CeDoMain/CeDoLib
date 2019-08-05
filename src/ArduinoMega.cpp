#include "ArduinoMega.h"

#ifdef ARDUINO_AVR_MEGA2560

DigitalOut* ArduinoMega::GetDigitalOut(const DigitalPin pin)
{
    // Wrapper für einen Digitalausgang erzeugen
    pinMode(pin, OUTPUT);
    return new DigitalOut([pin](bool value) 
    {
        digitalWrite(pin, value ? HIGH : LOW);
    });
}
DigitalIn* ArduinoMega::GetDigitalIn(const DigitalPin pin, const bool isPulledUp)
{
    // Wrapper für einen Digitaleingang erzeugen und Pullup konfigurieren
    pinMode(pin, isPulledUp ? INPUT_PULLUP : INPUT);
    return new DigitalIn([pin, isPulledUp]() -> bool
    {
        return digitalRead(pin) == HIGH;
    });
}
AnalogOut* ArduinoMega::GetAnalogOut(const AnalogOutPin pin)
{
    // Wrapper für einen Analogausgang erzeugen
    pinMode(pin, OUTPUT);
    return new AnalogOut([pin](float value) 
    {
        return analogWrite(pin, (byte)(value * 255));
    });
}
AnalogIn* ArduinoMega::GetAnalogIn(const AnalogInPin pin, AnalogReference reference)
{
    // Wrapper für einen Analogeingang erzeugen und Referenzspannung konfigurieren
    analogReference(reference);
    return new AnalogIn([pin]() -> float
    {
        return analogRead(pin) / 1023.0;
    });
}

#endif
