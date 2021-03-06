#include "MCP23017.h"

MCP23017::MCP23017(const byte A2A1A0)
	: I2CAddress(BASE_ADDRESS | (A2A1A0 & 0x07)),
	  Direction(0),			// alle Pins sind Ausgang
	  Pullup(0),			// kein Pullup
	  Input(0),				// kein Input
	  Output(0)				// alle pins auf LOW schalten
{

}

void MCP23017::Begin()
{
	// I2C Bus starten
	Wire.begin();

	// Chip mit Cache syncronisieren
	WriteWordRegister(REG_DIRECTION, Direction);
	WriteWordRegister(REG_PULLUP, Pullup);
	WriteWordRegister(REG_OUTPUT, Output);
}
void MCP23017::Update()
{
	// Eingangs Register vom Chip auslesen und im Cache speichern
  	Input = ReadWordRegister(REG_INPUT);
}

DigitalIn* MCP23017::GetDigitalIn(const IOPort pin, const bool isPulledUp)
{
	// Wrapper für einen Eingang erzeugen und Eingang konfigurieren
	PinMode(pin, isPulledUp ? INPUT_PULLUP : INPUT);
	return new DigitalIn([this, pin]() -> bool
	{
		return ISSET(Input, pin);
	});
}
DigitalOut* MCP23017::GetDigitalOut(const IOPort pin)
{
	// Wrapper für einen Ausgang erzeugen und Ausgang konfigurieren
	PinMode(pin, OUTPUT);
	return new DigitalOut([this, pin](const bool value)
	{
		if(value)
			SET(Output, pin);
		else
			UNSET(Output, pin);

		WriteWordRegister(REG_OUTPUT, Output);
	});
}

void MCP23017::PinMode(const IOPort pin, const int mode)
{
	switch (mode)
	{
	case INPUT_PULLUP:
		// Pullupwiderstand aktivieren
		SET(Pullup, pin);
		WriteWordRegister(REG_PULLUP, Pullup);
	case INPUT:
		// Port als Eingang konfigurieren
		SET(Direction, pin);
		break;

	case OUTPUT:
		// Port als Ausgang konfigurieren
		UNSET(Direction, pin);
		break;

	default:
		return;
	}

	// Register syncronisieren
	WriteWordRegister(REG_DIRECTION, Direction);
}

void MCP23017::WriteWordRegister(const byte regAddress, const word data)
{
	// 2 Bytes in angegebenes Register schreiben
	Wire.beginTransmission(I2CAddress);
	Wire.write(regAddress);
	Wire.write(highByte(data));
	Wire.write(lowByte(data));
	Wire.endTransmission();
}

void MCP23017::WriteByteRegister(const byte regAddress, const byte data)
{
	// 1 Byte in angegebenes Register schreiben
	Wire.beginTransmission(I2CAddress);
	Wire.write(regAddress);
	Wire.write(data);
	Wire.endTransmission();
}

word MCP23017::ReadWordRegister(const byte regAddress)
{
	// 2 Bytes aus angegebenem Register anfordern
	int value = 0x0000;
	Wire.beginTransmission(I2CAddress);
	Wire.write(regAddress);
	Wire.endTransmission();
	Wire.requestFrom((int)I2CAddress, 2);

	// Word (2 Byte) einlesen
	if(Wire.available())
	{
		value = Wire.read() << 8;
		value |= Wire.read();
	}

	return value;
}
