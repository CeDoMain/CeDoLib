#include "RFM69HCW.h"

RFM69HCW::RFM69HCW()
	: RegOperationMode(0x04),					// Standbymodus
      RegDataModulation(0x08),					// Packet Mode OOK NoShaping
      RegBitrate(0x1A0B),						// 4.8 kb/s
      RegFrequencyDevitation(0x0052),			// 5 kHz
      RegCarrierFrequency{ 0xE4, 0xC0, 0x00 },  // 915 MHz
      RegListen{ 0x90, 0xF5, 0x20 },			// Standard Timing, Nach PacketAcceptance in Rx bleiben
      RegPaLevel(0x5F),							// PA1, +13 dBm
	  RegRxBw(0x20),							// Rx Filter DC-CutOff 8% of Bw, Bw = 250 kHz
	  RegAfcBw(0x20),							// Afc Filter DC-CutOff 8% of Bw, Bw = 250 kHz
	  RegRssiThresh(0xE4),						// RSSI Threshold -114 dBm
      RegPreambleSize(0x0003),					// 3 Byte Preamble
      RegSyncConfig(0x00),						// Kein SyncWord
      RegPacketConfig1(0x00),					// Fixed Length, NoDcFree-Encoding, NoCrc, ClearIfCrcFailed, NoAddressFiltering
      RegPayloadLength(0x40),					// 64bit Packets
      RegNodeAddress(0x00),						// Adresse 0
      RegBroadcastAddress(0x00),				// Gruppe 0
      RegPacketConfig2(0x40),					// NoAes, NoAutoRxRestart, InterPacketdelay = 8 / Bitrate
	  NeedBoostMode(false),
	  NeedPA0(false),
	  TemperatureCalibrationValue(171)
{
	
}

void RFM69HCW::Begin(DigitalOut* pin_RST, DigitalOut* pin_CS, DigitalIn* pin_DIO0)
{
	SPI.begin();
	
	Pin_RST = pin_RST;
	Pin_CS = pin_CS;
	Pin_PayloadReady = pin_DIO0;
    (*Pin_CS)(true);

	// Reset
    (*Pin_RST)(true);
	delay(50);
    (*Pin_RST)(false);
	delay(50);

	// In Standby versetzen
	UpdateRegByte(REG_OPERATION_MODE, RegOperationMode);
	EnterStandbyMode();

	// Register programmieren
	UpdateRegByte(REG_DATA_MODULATION, RegDataModulation);
	UpdateRegWord(REG2_BITRATE, RegBitrate);
	UpdateRegWord(REG2_FREQUENCY_DEVITATION, RegFrequencyDevitation);
	UpdateRegArray(REG3_CARRIER_FREQUENCY, RegCarrierFrequency, 3);
	UpdateRegArray(REG3_LISTEN, RegListen, 3);
	UpdateRegByte(REG_PA_LEVEL, RegPaLevel);
	UpdateRegByte(REG_LNA, 0x80);							// LNA Impedanz 200 Ohm
	UpdateRegByte(REG_RX_BW, RegRxBw);
	UpdateRegByte(REG_AFC_BW, RegAfcBw);
	UpdateRegByte(REG_AFC_FEI, 0x14);						// Wenn Rx-Modus gestartet wird AFC ausführen
	UpdateRegByte(REG_DIO_MAPPING1, 0x5A);					// DIO0(Payload/TxReady), DIO1(FifoFull), DIO2(Deaktiviert), DIO3(Deaktiviert)
	UpdateRegByte(REG_DIO_MAPPING2, 0xA7);					// DIO5(Deaktiviert), No CLKOUT
	UpdateRegByte(REG_RSSI_THRESH, RegRssiThresh);
	UpdateRegWord(REG2_PREAMBLE_SIZE, RegPreambleSize);
	UpdateRegByte(REG_SYNC_CONFIG, RegSyncConfig);
	UpdateRegByte(REG_PACKET_CONFIG1, RegPacketConfig1);
	UpdateRegByte(REG_PAYLOAD_LENGTH, RegPayloadLength);
	UpdateRegByte(REG_NODE_ADDRESS, RegNodeAddress);
	UpdateRegByte(REG_BROADCAST_ADDRESS, RegBroadcastAddress);
	UpdateRegByte(REG_FIFO_THRESH, 0x8F);					// TxStart wenn FifoNotEmpty
	UpdateRegByte(REG_PACKET_CONFIG2, RegPacketConfig2);
	EnableBoostMode(false);									// Normale Sendeleistung
	UpdateRegByte(REG_TEST_DAGC, 0x20);						// DAGC konfigurieren
}

void RFM69HCW::Update()
{
	if ((*Pin_PayloadReady)())
	{
		EnterStandbyMode();

		// Es liegen Daten vor, die gelesen werden können
		byte message[66];

		// Daten einlesen
		byte count = ReadFifo(message);

		// Event auslösen
		if (MessageReceivedEvent != 0)
			(*MessageReceivedEvent)(message, count);
	}
}

void RFM69HCW::EnterSleepMode()
{
	SetMode(Mode::Sleep);
}

void RFM69HCW::EnterStandbyMode()
{
	SetMode(Mode::Standby);
}

void RFM69HCW::EnterReceiveMode()
{
	UpdateRegBit(REG_PA_LEVEL, false, BIT_PA0_ON, &RegPaLevel);
	SetMode(Mode::Receiver);
}

void RFM69HCW::EnterListenMode()
{
	UpdateRegBit(REG_PA_LEVEL, false, BIT_PA0_ON, &RegPaLevel);
	SetMode(Mode::Listen);
}

bool RFM69HCW::TransmitMessage(byte* message)
{
	// Standby aktivieren
	SetMode(Mode::Standby);
	UpdateRegBit(REG_PA_LEVEL, NeedPA0, BIT_PA0_ON, &RegPaLevel);

	// Konfigurierte Paketlänge berechnen und prüfen
	byte count = ISSET(RegPacketConfig1, BIT_PACKET_FORMAT) ? message[0] + 1 : RegPayloadLength;
	if (count > 64
		+ (ISSET(RegPacketConfig1, BIT_PACKET_FORMAT) ? 1 : 0)
		+ (EXTRACT(RegPacketConfig1, BIT2_ADDRESS_FILTERING, 2) != AddressFiltering::NoAddress ? 1 : 0))
		return false;

	// Fifo leeren, Daten hochladen und versenden
	UpdateRegBit(REG_IRQ_FLAGS2, true, BIT_FIFO_OVERRUN);
    WriteFifo(message, count);
	if (NeedBoostMode)
		EnableBoostMode(true);
	SetMode(Mode::Transmitter);

	// Nachdem gesendet wurde zurück in Standby
	while (ISUNSET(ReadByte(REG_IRQ_FLAGS2), BIT_PACKET_SENT));
	if (NeedBoostMode)
		EnableBoostMode(false);
	SetMode(Mode::Standby);

	return true;
}

void RFM69HCW::ConfigRadio(float mhzFrequency)
{
	// Trägerfrequenz in 3 Byte umrechnen
	long regValue = (long)(mhzFrequency * 1000000 / (32000000.0 / 524288) + 0.5);
	regValue = max(0x000000, min(regValue, 0xFFFFFF));
	byte NewRegCarrierFrequency[3] =
	{
		(byte)((regValue >> 16) & 0xFF),
		(byte)((regValue >> 8) & 0xFF),
		(byte)(regValue & 0xFF)
	};

	UpdateRegArray(REG3_CARRIER_FREQUENCY, NewRegCarrierFrequency, 3, RegCarrierFrequency);
}

void RFM69HCW::ConfigMessageLength(bool isLengthFixed, byte messageLength)
{
	UpdateRegBit(REG_PACKET_CONFIG1, !isLengthFixed, BIT_PACKET_FORMAT, &RegPacketConfig1);
	UpdateRegByte(REG_PAYLOAD_LENGTH, messageLength, &RegPayloadLength);
}

void RFM69HCW::ConfigListenMode(Resolution idleTimeRes, byte idleTimeFactor, Resolution rxTimeRes, byte rxTimeFactor)
{
	idleTimeFactor = max(idleTimeFactor, 1);
	rxTimeFactor = max(rxTimeFactor, 1);

	byte NewRegListen[3] = { RegListen[0], RegListen[1], RegListen[2] };
	COPY(NewRegListen[0], BIT2_LISTEN_RESOLUTION_IDLE, 2, idleTimeRes);
	NewRegListen[1] = idleTimeFactor;
	COPY(NewRegListen[0], BIT2_LISTEN_RESOLUTION_RX, 2, rxTimeRes);
	NewRegListen[2] = rxTimeFactor;
	
	UpdateRegArray(REG3_LISTEN, NewRegListen, 3, RegListen);
}

void RFM69HCW::ConfigAddress(byte nodeAddress, byte groupAddress, AddressFiltering filter)
{
	UpdateRegByte(REG_NODE_ADDRESS, nodeAddress, &RegNodeAddress);
	UpdateRegByte(REG_BROADCAST_ADDRESS, groupAddress, &RegBroadcastAddress);
	UpdateRegBits(REG_PACKET_CONFIG1, filter, BIT2_ADDRESS_FILTERING, 2, &RegPacketConfig1);
}

void RFM69HCW::ConfigSyncWord(byte toleratedWrongSyncWordBits,
	byte syncByte1,	byte syncByte2, byte syncByte3, byte syncByte4,
	byte syncByte5, byte syncByte6, byte syncByte7, byte syncByte8)
{
	if (syncByte1 == 0)
	{
		// SyncWord deaktivieren
		UpdateRegBit(REG_SYNC_CONFIG, false, BIT_SYNC_ON, &RegSyncConfig);
		UpdateRegBit(REG3_LISTEN, false, BIT_LISTEN_CRITERIA, RegListen);
		return;
	}

	byte NewRegSyncConfig = RegSyncConfig;
	byte RegSyncValue[8] = { syncByte1, syncByte2, syncByte3, syncByte4, syncByte5, syncByte6, syncByte7, syncByte8 };

	// Länge des SyncWords herausfinden
	for (byte i = 1; i <= 8; i++)
	{
		if (i == 8 || RegSyncValue[i] == 0)
		{
			COPY(NewRegSyncConfig, BIT3_SYNC_SIZE, 3, i - 1);
			UpdateRegArray(REG8_SYNC_VALUE, RegSyncValue, i);
			break;
		}
	}
	COPY(NewRegSyncConfig, BIT3_SYNC_TOLERANCE, 3, max(0, min(toleratedWrongSyncWordBits, 7)));
	SET(NewRegSyncConfig, BIT_SYNC_ON);
	UpdateRegBit(REG3_LISTEN, true, BIT_LISTEN_CRITERIA, RegListen);
	UpdateRegByte(REG_SYNC_CONFIG, NewRegSyncConfig, &RegSyncConfig);
}

void RFM69HCW::ConfigCrc(bool isCrcEnabled)
{
	UpdateRegBit(REG_PACKET_CONFIG1, isCrcEnabled, BIT_CRC_ON, &RegPacketConfig1);
}

void RFM69HCW::ConfigEncoding(Encoding code, byte aesKey[16])
{
	UpdateRegBits(REG_PACKET_CONFIG1, code, BIT2_DC_FREE, 2, &RegPacketConfig1);
	if (aesKey != 0)
	{
		UpdateRegArray(REG16_AES_KEY, aesKey, 16);
		UpdateRegBit(REG_PACKET_CONFIG2, true, BIT_AES_ON, &RegPacketConfig2);
	}
	else
	{
		UpdateRegBit(REG_PACKET_CONFIG2, false, BIT_AES_ON, &RegPacketConfig2);
	}
}

void RFM69HCW::SetPreambleLength(word length)
{
	UpdateRegWord(REG2_PREAMBLE_SIZE, length, &RegPreambleSize);
}

void RFM69HCW::EnableFskModulation(FskShaping shaping, float kHzFrequencyDevitation, float kbpsBitRate)
{
	// neue Registerwerte berechnen
	byte NewRegDataModulation = RegDataModulation;
	COPY(NewRegDataModulation, BIT2_MODULATION_TYPE, 2, ModulationType::Fsk);
	COPY(NewRegDataModulation, BIT2_MODULATION_SHAPING, 2, shaping);

	kHzFrequencyDevitation = max(0.6, min(kHzFrequencyDevitation, max(300, 500 - kbpsBitRate / 2)));
	word RegValueFreqDev = (word)(kHzFrequencyDevitation / (32000000.0 / 524288) + 0.5);
	kbpsBitRate = max(1.2, min(kbpsBitRate, 300));
	word RegValueBitRate = (word)(32000 / kbpsBitRate + 0.5);

	// Filterwerte berechnen (Rx > BitRate, Afc > 5 * BitRate)
	byte NewRegRxBw = 0x40;
	byte NewRegAfcBw = 0x80;
	for (byte e = 7; e < 8; e--)
	{
		for (byte m = 24; m >= 16; m -= 4)
		{
			if (NewRegRxBw == 0x40 && 32000.0 / m / pow(2, e + 2) > kbpsBitRate)
			{
				COPY(NewRegRxBw, BIT2_BW_MANT, 2, (m - 16) / 4);
				COPY(NewRegRxBw, BIT3_BW_EXP, 3, e);
			}
			if (NewRegAfcBw == 0x80 && 32000.0 / m / pow(2, e + 2) > 5 * kbpsBitRate)
			{
				COPY(NewRegAfcBw, BIT2_BW_MANT, 2, (m - 16) / 4);
				COPY(NewRegAfcBw, BIT3_BW_EXP, 3, e);
			}
		}
	}

	UpdateRegByte(REG_OPERATION_MODE, NewRegDataModulation, &RegDataModulation);
	UpdateRegWord(REG2_FREQUENCY_DEVITATION, RegValueFreqDev, &RegFrequencyDevitation);
	UpdateRegWord(REG2_BITRATE, RegValueBitRate, &RegBitrate);
	UpdateRegByte(REG_RX_BW, NewRegRxBw, &RegRxBw);
	UpdateRegByte(REG_AFC_BW, NewRegAfcBw, &RegAfcBw);
}

void RFM69HCW::EnableOokModulation(OokShaping shaping, float kbpsBitRate)
{
	// neue Registerwerte berechnen
	byte NewRegDataModulation = RegDataModulation;
	COPY(NewRegDataModulation, BIT2_MODULATION_TYPE, 2, ModulationType::Ook);
	COPY(NewRegDataModulation, BIT2_MODULATION_SHAPING, 2, shaping);

	kbpsBitRate = max(1, min(kbpsBitRate, 32768));
	word RegValueBitRate = (word)(32000 / kbpsBitRate + 0.5);

	UpdateRegByte(REG_OPERATION_MODE, NewRegDataModulation, &RegDataModulation);
	UpdateRegWord(REG2_BITRATE, RegValueBitRate, &RegBitrate);
	UpdateRegByte(REG_RX_BW, 0x20, &RegRxBw);
	UpdateRegByte(REG_AFC_BW, 0x20, &RegAfcBw);
}

void RFM69HCW::SetOutputPower(signed char dBmPower)
{
	dBmPower = max(-18, min(dBmPower, 20));
	byte NewRegPaLevel = 0x00;
	NeedBoostMode = false;
	NeedPA0 = false;
	if (dBmPower < -2)
	{
		// -18 bis -3 dBm: PA0 auf RFIO aktivieren
		SET(NewRegPaLevel, BIT_PA0_ON);
		COPY(NewRegPaLevel, BIT5_OUTPUT_POWER, 5, dBmPower + 18);
		NeedPA0 = true;
	}
	else if (dBmPower < 14)
	{
		// -2 bis +13 dBm: PA1 auf PA_BOOST aktivieren
		SET(NewRegPaLevel, BIT_PA1_ON);
		COPY(NewRegPaLevel, BIT5_OUTPUT_POWER, 5, dBmPower + 18);
	}
	else if (dBmPower < 18)
	{
		// +14 bis +17 dBm: PA1 und PA2 auf PA_BOOST aktivieren
		SET(NewRegPaLevel, BIT_PA1_ON);
		SET(NewRegPaLevel, BIT_PA2_ON);
		COPY(NewRegPaLevel, BIT5_OUTPUT_POWER, 5, dBmPower + 14);
	}
	else
	{
		// +18 bis +20 dBm: PA1, PA2 und BOOST-Modus auf PA_BOOST aktivieren
		SET(NewRegPaLevel, BIT_PA1_ON);
		SET(NewRegPaLevel, BIT_PA2_ON);
		COPY(NewRegPaLevel, BIT5_OUTPUT_POWER, 5, dBmPower + 11);
		NeedBoostMode = true;
	}
	UpdateRegByte(REG_PA_LEVEL, NewRegPaLevel, &RegPaLevel);
}

void RFM69HCW::SetHighSensitivityMode(bool isEnabled)
{
	UpdateRegByte(REG_TEST_LNA, isEnabled ? 0x2D : 0x1B);
}

float RFM69HCW::MeasureInputPower()
{
	return ReadByte(REG_RSSI_VALUE) * -0.5;
}

float RFM69HCW::MeasureFrequencyError()
{
	// 2er Komplementdarstellung interpretieren
	float value = ReadWord(REG2_FEI_VALUE);
	value = value >= 0x8000 ? value - 0x10000 : value;
	return value * (32000000.0 / 524288) / 1000000;
}

short RFM69HCW::MeasureTemperature(short realTemperature)
{
	EnterStandbyMode();
	UpdateRegBit(REG_TEMP1, true, BIT_TEMP_MEAS_START);
	while (ISSET(ReadByte(REG_TEMP1), BIT_TEMP_MEAS_RUNNING));
	short value = (short)ReadByte(REG_TEMP2);
	if (realTemperature != 127)
	{
		// Temperatur soll kalibriert werden
		TemperatureCalibrationValue = realTemperature + value;
	}
	return -value + TemperatureCalibrationValue;
}

void RFM69HCW::CalibrateInternalOscillator()
{
	EnterStandbyMode();
	UpdateRegBit(REG_OSC1, true, BIT_RC_CAL_START);
	while (ISUNSET(ReadByte(REG_OSC1), BIT_RC_CAL_DONE));
}

float RFM69HCW::SetRxBw(bool inc)
{
	byte NewRegRxBw = RegRxBw;
	byte m = EXTRACT(RegRxBw, BIT2_BW_MANT, 2);
	byte e = EXTRACT(RegRxBw, BIT3_BW_EXP, 3);
	if (inc)
	{
		m = (m == 0 ? 2 : m - 1);
		e = (m == 2 ? (e == 0 ? 7 : e - 1) : e);
	}
	else
	{
		m = (m == 2 ? 0 : m + 1);
		e = (m == 0 ? (e == 7 ? 0 : e + 1) : e);
	}
	COPY(NewRegRxBw, BIT2_BW_MANT, 2, m);
	COPY(NewRegRxBw, BIT3_BW_EXP, 3, e);

	UpdateRegByte(REG_RX_BW, NewRegRxBw, &RegRxBw);

	return 32000.0 / (m * 4 + 16) / pow(2, e + 3);
}

float RFM69HCW::SetRxDccFreq(bool inc)
{
	byte f = EXTRACT(RegRxBw, BIT3_DCC_FREQ, 3);
	f = inc ? (f == 0 ? 7 : f - 1) : (f == 7 ? 0 : f + 1);
	UpdateRegBits(REG_RX_BW, f, BIT3_DCC_FREQ, 3, &RegRxBw);
	return 16.0 / pow(2, f);
}

float RFM69HCW::SetAfcBw(bool inc)
{
	byte NewRegAfcBw = RegAfcBw;
	byte m = EXTRACT(RegAfcBw, BIT2_BW_MANT, 2);
	byte e = EXTRACT(RegAfcBw, BIT3_BW_EXP, 3);
	if (inc)
	{
		m = (m == 0 ? 2 : m - 1);
		e = (m == 2 ? (e == 0 ? 7 : e - 1) : e);
	}
	else
	{
		m = (m == 2 ? 0 : m + 1);
		e = (m == 0 ? (e == 7 ? 0 : e + 1) : e);
	}
	COPY(NewRegAfcBw, BIT2_BW_MANT, 2, m);
	COPY(NewRegAfcBw, BIT3_BW_EXP, 3, e);

	UpdateRegByte(REG_RX_BW, NewRegAfcBw, &RegAfcBw);

	return 32000.0 / (m * 4 + 16) / pow(2, e + 3);
}

float RFM69HCW::SetAfcDccFreq(bool inc)
{
	byte f = EXTRACT(RegAfcBw, BIT3_DCC_FREQ, 3);
	f = inc ? (f == 0 ? 7 : f - 1) : (f == 7 ? 0 : f + 1);
	UpdateRegBits(REG_RX_BW, f, BIT3_DCC_FREQ, 3, &RegAfcBw);
	return 16.0 / pow(2, f);
}

word RFM69HCW::GetIrqFlags()
{
	return ReadWord(REG_IRQ_FLAGS1);
}

float RFM69HCW::SetRssiThresh(signed char change)
{
	RegRssiThresh += (RegRssiThresh != 0 && RegRssiThresh != 255 ? change : 0);
	UpdateRegByte(REG_RSSI_THRESH, RegRssiThresh);
	return RegRssiThresh * -0.5;
}

void RFM69HCW::SetMode(Mode mode)
{
	byte NewRegOperationMode = RegOperationMode;
	if (mode != Mode::Listen)
	{
		if (ISSET(NewRegOperationMode, BIT_LISTEN_ON))
		{
			// Chip im Empfangsmodus > Empfangsmodus abbrechen
			UNSET(NewRegOperationMode, BIT_LISTEN_ON);
			SET(NewRegOperationMode, BIT_LISTEN_ABORT);
			UpdateRegByte(REG_OPERATION_MODE, NewRegOperationMode, &RegOperationMode);
			UNSET(NewRegOperationMode, BIT_LISTEN_ABORT);
		}
	}
	else
	{
		// Empfangsmodus aktivieren
		SET(NewRegOperationMode, BIT_LISTEN_ON);
	}
	COPY(NewRegOperationMode, BIT3_MODE, 3, mode);

	UpdateRegByte(REG_OPERATION_MODE, NewRegOperationMode, &RegOperationMode);

	while (ISUNSET(ReadByte(REG_IRQ_FLAGS1), BIT_MODE_READY));
}

void RFM69HCW::EnableBoostMode(bool isBoosted)
{
	UpdateRegByte(REG_OCP, isBoosted ? 0x0F : 0x1A);
	UpdateRegByte(REG_TEST_PA1, isBoosted ? 0x5D : 0x55);
	UpdateRegByte(REG_TEST_PA2, isBoosted ? 0x7C : 0x70);
}

void RFM69HCW::SpiBegin()
{	
    // Chipselect aktivieren (LOW active) und Bus konfigurieren
    SPI.beginTransaction(SPISettings(32000000, MSBFIRST, SPI_MODE0));
    (*Pin_CS)(false);
}

void RFM69HCW::SpiEnd()
{
    // Übertragung beenden
    (*Pin_CS)(true);
    SPI.endTransaction();
}

void RFM69HCW::UpdateRegBit(byte regAddress, bool newBit, byte bit, byte* oldData)
{
	byte tmp = 0x00;
	if (oldData == 0)
		oldData = &tmp;

	if (ISSET(*oldData, bit) != newBit)
	{
		// Register muss geändert werden
    	newBit ? SET(*oldData, bit) : UNSET(*oldData, bit);

		SET(regAddress, 7);
		SpiBegin();
		SPI.transfer(regAddress);
		SPI.transfer(*oldData);
		SpiEnd();
	}
}

void RFM69HCW::UpdateRegBits(byte regAddress, byte newBits, byte bit, byte count, byte* oldData)
{
	byte tmp = 0x00;
	if (oldData == 0)
		oldData = &tmp;

    if (EXTRACT(*oldData, bit, count) != newBits)
    {
        // Register muss geändert werden
        COPY(*oldData, bit, count, newBits);
		
		SET(regAddress, 7);
		SpiBegin();
		SPI.transfer(regAddress);
		SPI.transfer(*oldData);
		SpiEnd();
	}
}

void RFM69HCW::UpdateRegByte(byte regAddress, byte newByte, byte* oldData)
{
    if (oldData == 0 || *oldData != newByte)
    {
        // Register muss geändert werden
        if (oldData != 0)
			*oldData = newByte;
		
		SET(regAddress, 7);
		SpiBegin();
		SPI.transfer(regAddress);
		SPI.transfer(newByte);
		SpiEnd();
	}
}

void RFM69HCW::UpdateRegWord(byte regAddress, word newWord, word* oldData)
{
    if (oldData == 0 || *oldData != newWord)
    {
        // Register muss geändert werden
        if (oldData != 0)
			*oldData = newWord;
		
		SET(regAddress, 7);
		SpiBegin();
		SPI.transfer(regAddress);
		SPI.transfer16(newWord);
		SpiEnd();
    }
}

void RFM69HCW::UpdateRegArray(byte regAddress, byte* newData, byte count, byte* oldData)
{
    for (byte i = 0; i < count; i++)
    {
        if (oldData == 0 || oldData[i] != newData[i])
        {
        	// Register muss geändert werden
			SET(regAddress, 7);
			SpiBegin();
			SPI.transfer(regAddress + i);

            for (; i < count; i++)
            {
				SPI.transfer(newData[i]);
				if (oldData != 0)
					oldData[i] = newData[i];
            }
			SpiEnd();
			break;
        }
    }
}

byte RFM69HCW::ReadByte(byte regAddress)
{
	UNSET(regAddress, 7);
	SpiBegin();
	SPI.transfer(regAddress);
	byte value = SPI.transfer(0xFF);
	SpiEnd();
	return value;
}

word RFM69HCW::ReadWord(byte regAddress)
{
	UNSET(regAddress, 7);
	SpiBegin();
	SPI.transfer(regAddress);
	word value = SPI.transfer16(0xFFFF);
	SpiEnd();
	return value;
}

void RFM69HCW::WriteFifo(byte* data, byte count)
{
	byte buffer[66];
	memcpy(buffer, data, count);
	SpiBegin();
	SPI.transfer(0x80);
	SPI.transfer(buffer, count);
	SpiEnd();
}

byte RFM69HCW::ReadFifo(byte* data)
{
	SpiBegin();
	SPI.transfer(0x00);
	data[0] = SPI.transfer(0xFF);
	byte count = min(ISSET(RegPacketConfig1, BIT_PACKET_FORMAT) ? data[0] + 1 : RegPayloadLength, 66);
	for (byte i = 1; i < count; i++)
	{
		data[i] = SPI.transfer(0xFF);
	}
	SpiEnd();
	return count;
}
