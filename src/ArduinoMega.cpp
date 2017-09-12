#include "ArduinoMega.h"

DigitalOut* ArduinoMega::GetDigitalOut(const DigitalPin pin)
{
  pinMode(pin, OUTPUT);
  return new DigitalOut([pin](const bool value) 
  {
    digitalWrite(pin, value ? HIGH : LOW);
  });
}
DigitalIn* ArduinoMega::GetDigitalIn(const DigitalPin pin, const bool isPulledUp)
{
  pinMode(pin, isPulledUp ? INPUT_PULLUP : INPUT);
  return new DigitalIn([pin, isPulledUp]() -> bool
  {
    return digitalRead(pin) == HIGH;
  });
}
AnalogOut* ArduinoMega::GetAnalogOut(const AnalogOutPin pin)
{
  pinMode(pin, OUTPUT);
  return new AnalogOut([pin](const float value) 
  {
    return analogWrite(pin, (byte)(value * 255));
  });
}
AnalogIn* ArduinoMega::GetAnalogIn(const AnalogInPin pin, AnalogReference reference)
{
  analogReference(reference);
  return new AnalogIn([pin]() -> bool
  {
    return analogRead(pin / 1023.0);
  });
}
