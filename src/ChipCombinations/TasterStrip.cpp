#include "TasterStrip.h"

TasterStrip::TasterStrip(byte A2A1A0)
    : PWMchip(B111000 | (A2A1A0 & B111)), GPIOchip(A2A1A0 & B111)
{
    // Maskierung I2C Adresse (alternativ zu oben)
    // PWMchip = PCA9685(B111000 | (A2A1A0 & B111)); 
    // GPIOchip = MCP23017(A2A1A0 & B111);
}

void TasterStrip::Begin()
{
    // Chips initialisieren
    GPIOchip.Begin();
    PWMchip.Begin();
   
    // LED-Pins verknüpfen
    Led[0].Begin(PWMchip.GetAnalogOut(PCA9685::LED::LED9), PWMchip.GetAnalogOut(PCA9685::LED::LED8));
    Led[1].Begin(PWMchip.GetAnalogOut(PCA9685::LED::LED11), PWMchip.GetAnalogOut(PCA9685::LED::LED10));
    Led[2].Begin(PWMchip.GetAnalogOut(PCA9685::LED::LED13), PWMchip.GetAnalogOut(PCA9685::LED::LED12));
    Led[3].Begin(PWMchip.GetAnalogOut(PCA9685::LED::LED1), PWMchip.GetAnalogOut(PCA9685::LED::LED0));
    Led[4].Begin(PWMchip.GetAnalogOut(PCA9685::LED::LED3), PWMchip.GetAnalogOut(PCA9685::LED::LED2));

    // GPI-Pins verknüpfen
    Btn[0].Begin(new Trigger(GPIOchip.GetDigitalIn(MCP23017::IOPort::GPB0)));
    Btn[1].Begin(new Trigger(GPIOchip.GetDigitalIn(MCP23017::IOPort::GPB1)));
    Btn[2].Begin(new Trigger(GPIOchip.GetDigitalIn(MCP23017::IOPort::GPB2)));
    Btn[3].Begin(new Trigger(GPIOchip.GetDigitalIn(MCP23017::IOPort::GPA6)));
    Btn[4].Begin(new Trigger(GPIOchip.GetDigitalIn(MCP23017::IOPort::GPA7)));
}

void TasterStrip::Update()
{
    GPIOchip.Update();
    for (int i = 0; i < 5; i++)
    {
        Led[i].Update();
        Btn[i].Update();
    }
}

void TasterStrip::AllLedOn()
{
    Led[0].On();
    Led[1].On();
    Led[2].On();
    Led[3].On();
    Led[4].On();
}
