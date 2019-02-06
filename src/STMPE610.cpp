#include "STMPE610.h"

STMPE610::STMPE610()
    : WasTouched(false),
      Rotation(1),
      ReleaseTimeStamp(0)
{
    
}

void STMPE610::Begin(DigitalOut* pin_CS)
{
	SPI.begin();
	Pin_CS = pin_CS;
    (*Pin_CS)(true);

	// Reset
    WriteRegByte(REG_SYS_CTRL1, 0x02);      // Reset ausführen
	delay(10);
    
    WriteRegByte(REG_SPI_CFG, 0x01);        // MODE1
    
	WriteRegByte(REG_SYS_CTRL2, 0x04);      // Clocks starten
    WriteRegByte(REG_TSC_CTRL, 0x03);       // Kein Tracking, XY und TSC aktivieren
    WriteRegByte(REG_ADC_CTRL1, 0x50);      // 96 Clock-Pulse pro Messung, 10 bit
    WriteRegByte(REG_ADC_CTRL2, 0x02);      // ADC Clock 6.5 MHz
    WriteRegByte(REG_TSC_CFG, 0xE4);        // 8 Samples, Delay 1 ms, Settling 5 ms
    WriteRegByte(REG_TSC_FRACTION_Z, 0x06); // 2 bit Ganzzahlteil, 6 bit Kommateil
    WriteRegByte(REG_TSC_I_DRIVE, 0x01);    // 50 mA Touchscreen Strom (max. 80 mA)
    WriteRegWord(REG_WDW_TR_X, 3820);       // Touchscreen auf Bildschirm begrenzen
    WriteRegWord(REG_WDW_TR_Y, 3760);
    WriteRegWord(REG_WDW_BL_X, 260);
    WriteRegWord(REG_WDW_BL_Y, 190);
}

void STMPE610::Update()
{
    if (TouchEvent == 0)
        return;

    if (!WasTouched && ISUNSET(ReadRegByte(REG_FIFO_CTRL_STA), BIT_FIFO_EMPTY))
    {
        // Touchscreen wurde gedrückt
        if (millis() - ReleaseTimeStamp < 100)
        {
            // Es ist noch nicht genug Zeit seit dem Loslassen vergangen
            ClearFifo();
        }
        else
        {
            // Genug Zeit vergangen, es kann ausgelöst werden
            unsigned short ra, rb, ta, tb;
            ReadFifo(&ra, &rb);

            // Gemessene Position in Pixel auf dem Bildschirm umrechnen
            switch (Rotation)
            {
                case 1:
                    ta = constrain(map(rb, 190, 3760, 0, 480), 0, 480);
                    tb = constrain(map(ra, 3820, 260, 0, 320), 0, 320);
                    break;

                default:
                    ta = ra;
                    tb = rb;
                    break;
            }
            (*TouchEvent)(ta, tb);
            WasTouched = true;
        }
    }
    else if (WasTouched && ISUNSET(ReadRegByte(REG_TSC_CTRL), BIT_TSC_STA))
    {
        // Touchscreen wurde losgelassen
        ReleaseTimeStamp = millis();
        ClearFifo();
        WasTouched = false;
    }
}

void STMPE610::SetRotation(byte r)
{
    Rotation = r;
}

void STMPE610::SpiBegin()
{	
    // Chipselect aktivieren (LOW active) und Bus konfigurieren
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    (*Pin_CS)(false);
}

void STMPE610::SpiEnd()
{
    // Übertragung beenden
    (*Pin_CS)(true);
    SPI.endTransaction();
}

void STMPE610::WriteRegByte(byte regAddress, byte newByte)
{
    UNSET(regAddress, 7);
    SpiBegin();
    SPI.transfer(regAddress);
    SPI.transfer(newByte);
    SpiEnd();
}

void STMPE610::WriteRegWord(byte regAddress, word newWord)
{
    WriteRegByte(regAddress, highByte(newWord));
    WriteRegByte(regAddress + 1, lowByte(newWord));
}

byte STMPE610::ReadRegByte(byte regAddress)
{
	SET(regAddress, 7);
	SpiBegin();
	SPI.transfer(regAddress);
	byte value = SPI.transfer(0x00);
	SpiEnd();
	return value;
}

void STMPE610::ReadFifo(unsigned short* x, unsigned short* y)
{
	SpiBegin();
	SPI.transfer(REG_TSC_DATA | 0x80);
	unsigned long value = SPI.transfer(REG_TSC_DATA);
    value <<= 8;
    value |= SPI.transfer(REG_TSC_DATA);
    value <<= 8;
    value |= SPI.transfer(0x00);
	SpiEnd();
    if (x != 0)
        *x = EXTRACT(value, 12, 12);
    if (y != 0)
        *y = EXTRACT(value, 0, 12);
}

void STMPE610::ClearFifo()
{
    WriteRegByte(REG_FIFO_CTRL_STA, 0x01);
    WriteRegByte(REG_FIFO_CTRL_STA, 0x00);
}