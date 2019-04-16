#include "PCA9685.h"

PCA9685::PCA9685(const byte A5A4A3A2A1A0)
  : I2CAddress(BASE_ADDRESS | (A5A4A3A2A1A0 & 0x3F))
{

}

void PCA9685::Begin(const decimal preScale)
{
	// I2C Bus starten
	Wire.begin();

	// PreScale für PWM setzen
	byte ps = map(constrain(preScale, 0, 10000), 0, 10000, 0xFF, 0x03);
	WriteByteRegister(REG_PRESCALE, ps);

	// Aufwecken und Auto-Increment der Registeradresse aktivieren
	WriteByteRegister(REG_MODE1, 0x21);

	// Output invertieren, Change on STOP, Open-Drain
	WriteByteRegister(REG_MODE2, 0x10);
}
DigitalOut* PCA9685::GetDigitalOut(const LED led)
{
	// Wrapper für Digitalausgang erzeugen
	return new DigitalOut([this, led](const bool value)
	{
		AnalogWrite(led, value ? 1 : 0);
	});
}
AnalogOut* PCA9685::GetAnalogOut(const LED led)
{
	// Wrapper für Analogausgang erzeugen
	return new AnalogOut([this, led](const float value)
	{
		AnalogWrite(led, value);
	});
}

void PCA9685::AnalogWrite(const LED led, const float value)
{
	// Register für die PWM Daten berechnen
	byte REG_LED_ON = (led == LED::ALL) ? REG_ALL_LED_ON : (REG_LED0_ON + led * 4);

	// OFF-Time berechnen
	short OFF_Time = (short)(value * (float)0xFFF) & 0xFFF;

	// Phase zufällig festlegen um den Gesamtstrom zu verteilen
	short Phase = (short)random(0x1000 - OFF_Time);

	// Registerwerte schreiben
	Wire.beginTransmission(I2CAddress);
	Wire.write(REG_LED_ON);
	Wire.write(lowByte(Phase));
	Wire.write(highByte(Phase));
	Wire.write(lowByte(Phase + OFF_Time));
	Wire.write(highByte(Phase + OFF_Time));
	Wire.endTransmission();
}

void PCA9685::WriteByteRegister(const byte regAddress, const byte data)
{
	// Byte in Register schreiben
	Wire.beginTransmission(I2CAddress);
	Wire.write(regAddress);
	Wire.write(data);
	Wire.endTransmission();
}
