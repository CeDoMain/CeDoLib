#include "EncoderStrip.h"

EncoderStrip::EncoderStrip(byte A2A1A0)
    :  Disp(), I2CAddressAtTiny(AtTinyBaseAddress | (A2A1A0 & 0x07)), Gpio(A2A1A0)
{
}

void EncoderStrip::Begin()
{
    Wire.setClock(400000L);
    Wire.begin();
    
    // Display verknüpfen (wird über I2C adressiert und über SPI beschickt)
    Gpio.Begin();
    Disp.Begin(
        Gpio.GetDigitalOut(MCP23017::GPA0),
        Gpio.GetDigitalOut(MCP23017::GPA2),
        Gpio.GetDigitalOut(MCP23017::GPA1),
        true, true);

    // Drucktaster der Encoder verknüpfen
    ButtonTop.Begin(new Trigger(new DigitalIn([this]() -> bool { return ButtonTopState; })));
    ButtonBtm.Begin(new Trigger(new DigitalIn([this]() -> bool { return ButtonBtmState; })));
}

void EncoderStrip::Update()
{
    // AtTiny auslesen
    if (Wire.requestFrom(I2CAddressAtTiny, 3) == 3)
    {
        // Daten empfangen
        byte data = Wire.read();
        int EncoderTopChange = -Wire.read() + 128;
        int EncoderBtmChange = -Wire.read() + 128;
    
        // Encoder Drehung verarbeiten
        if (EncoderTopChange != 0 && EncoderTopChangedEvent != 0)
            (*EncoderTopChangedEvent)(EncoderTopChange);
        if (EncoderBtmChange != 0 && EncoderBtmChangedEvent != 0)
            (*EncoderBtmChangedEvent)(EncoderBtmChange);
    
        // Encoder Taster verarbeiten
        ButtonTopState = ISSET(data, 0);
        ButtonBtmState = ISSET(data, 1);
    }
    ButtonTop.Update();
    ButtonBtm.Update();
}