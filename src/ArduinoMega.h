#ifndef ARDUINOMEGA_H
#define ARDUINOMEGA_H

#include "BaseIO.h"
#include "Arduino.h"

class ArduinoMega
{
  // Aufzählungen
public:
  enum DigitalPin
  {
    D0, D1,                                                 // Serielle Ports
    D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13,     // PWM Ports
    D14, D15, D16, D17, D18, D19, D20, D21,                 // Serielle und I2C Ports
    D22, D23, D24, D25, D26, D27, D28, D29, D30, D31,       // Digital Ports
    D32, D33, D34, D35, D36, D37, D38, D39, D40, D41,       // Digital Ports
    D42, D43,                                               // Digital Ports
    D44, D45, D46,                                          // PWM Ports
    D47, D48, D49,                                          // Digital Ports
    D50, D51, D52, D53                                      // SPI Ports
  };
  enum AnalogOutPin
  {
    PWM2 = 2, PWM3, PWM4, PWM5, PWM6, PWM7, PWM8, PWM9, PWM10, PWM11, PWM12, PWM13, PWM44 = 44, PWM45, PWM46
  };
  enum AnalogInPin
  {
    A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15
  };
  enum AnalogReference
  {
    Default = DEFAULT,
    Internal1V1 = INTERNAL1V1,
    Internal2V56 = INTERNAL2V56,
    External
  };

  // Methoden
public:
  static DigitalOut* GetDigitalOut(const DigitalPin pin);
  static DigitalIn* GetDigitalIn(const DigitalPin pin, const bool isPulledUp = true);
  static AnalogOut* GetAnalogOut(const AnalogOutPin pin);
  static AnalogIn* GetAnalogIn(const AnalogInPin pin, AnalogReference reference = AnalogReference::Default);
};

#endif
