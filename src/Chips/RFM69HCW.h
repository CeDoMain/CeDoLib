#ifndef RFM69HCW_H
#define RFM69HCW_H

#include "Arduino.h"
#include "SPI.h"
#include "BaseIO.h"

class RFM69HCW
{
    // Aufzählungen
public:
    enum Mode
    {
        Sleep = B000,
        Standby = B001,
        Transmitter = B011,
        Receiver = B100,
        Listen = B1001          // Entspricht keinem Wert im Register - für interne Zwecke
    };
    enum ModulationType
    {
        Fsk = B00,
        Ook = B01
    };
    enum FskShaping
    {
        RawFsk = B00,
        GaussianBt10 = B01,
        GaussianBt05 = B10,
        GaussianBt03 = B11
    };
    enum OokShaping
    {
        RawOok = B00,
        FCutOffBr = B01,
        FCutOff2Br = B10
    };
    enum Resolution
    {
        Res64us = B01,
        Res4100us = B10,
        Res262ms = B11
    };
    enum Encoding
    {
        NoEncoding = B00,
        Manchester = B01,
        Whitening = B10
    };
    enum AddressFiltering
    {
        NoAddress = B00,
        Node = B01,
        NodeAndBroadcast = B10
    };

    // Felder
public:
    Delegate<void, byte*, byte>* MessageReceivedEvent;      // Wird ausgelöst, wenn Daten empfangen wurden (Parameter: Nachricht, Anzahl Bytes)

private:
    static const byte REG_FIFO =                    0x00;
    static const byte REG_OPERATION_MODE =          0x01;
    static const byte BIT_LISTEN_ON =               6;
    static const byte BIT_LISTEN_ABORT =            5;
    static const byte BIT3_MODE =                   2;
    static const byte REG_DATA_MODULATION =         0x02;
    static const byte BIT2_MODULATION_TYPE =        3;
    static const byte BIT2_MODULATION_SHAPING =     0;
    static const byte REG2_BITRATE =                0x03;
    static const byte REG2_FREQUENCY_DEVITATION =   0x05;
    static const byte REG3_CARRIER_FREQUENCY =      0x07;
    static const byte REG_OSC1 =                    0x0A;
    static const byte BIT_RC_CAL_START =            7;
    static const byte BIT_RC_CAL_DONE =             6;
    static const byte REG3_LISTEN =                 0x0D;
    static const byte BIT_LISTEN_CRITERIA =         3;
    static const byte BIT2_LISTEN_RESOLUTION_IDLE = 6;
    static const byte BIT2_LISTEN_RESOLUTION_RX =   4;
    static const byte REG_PA_LEVEL =                0x11;
    static const byte BIT_PA0_ON =                  7;
    static const byte BIT_PA1_ON =                  6;
    static const byte BIT_PA2_ON =                  5;
    static const byte BIT5_OUTPUT_POWER =           0;
    static const byte REG_OCP =                     0x13;
    static const byte REG_LNA =                     0x18;
    static const byte REG_RX_BW =                   0x19;
    static const byte REG_AFC_BW =                  0x1A;
    static const byte BIT3_DCC_FREQ =               5;
    static const byte BIT2_BW_MANT =                3;
    static const byte BIT3_BW_EXP =                 0;
    static const byte REG_AFC_FEI =                 0x1E;
    static const byte REG2_FEI_VALUE =              0x21;
    static const byte REG_RSSI_CONFIG =             0x23;
    static const byte BIT_RSSI_DONE =               1;
    static const byte BIT_RSSI_START =              0;
    static const byte REG_RSSI_VALUE =              0x24;
    static const byte REG_DIO_MAPPING1 =            0x25;
    static const byte REG_DIO_MAPPING2 =            0x26;
    static const byte REG_IRQ_FLAGS1 =              0x27;
    static const byte BIT_MODE_READY =              7;
    static const byte REG_IRQ_FLAGS2 =              0x28;
    static const byte BIT_FIFO_NOT_EMPTY =          6;
    static const byte BIT_FIFO_OVERRUN =            4;
    static const byte BIT_PACKET_SENT =             3;
    static const byte REG_RSSI_THRESH =             0x29;
    static const byte REG2_PREAMBLE_SIZE =          0x2C;
    static const byte REG_SYNC_CONFIG =             0x2E;
    static const byte BIT_SYNC_ON =                 7;
    static const byte BIT3_SYNC_SIZE =              3;
    static const byte BIT3_SYNC_TOLERANCE =         0;
    static const byte REG8_SYNC_VALUE =             0x2F;
    static const byte REG_PACKET_CONFIG1 =          0x37;
    static const byte BIT_PACKET_FORMAT =           7;
    static const byte BIT2_DC_FREE =                5;
    static const byte BIT_CRC_ON =                  4;
    static const byte BIT2_ADDRESS_FILTERING =      1;
    static const byte REG_PAYLOAD_LENGTH =          0x38;
    static const byte REG_NODE_ADDRESS =            0x39;
    static const byte REG_BROADCAST_ADDRESS =       0x3A;
    static const byte REG_FIFO_THRESH =             0x3C;
    static const byte REG_PACKET_CONFIG2 =          0x3D;
    static const byte BIT_AES_ON =                  0;
    static const byte REG16_AES_KEY =               0x3E;
    static const byte REG_TEMP1 =                   0x4E;
    static const byte BIT_TEMP_MEAS_START =         3;
    static const byte BIT_TEMP_MEAS_RUNNING =       2;
    static const byte REG_TEMP2 =                   0x4F;
    static const byte REG_TEST_LNA =                0x58;
    static const byte REG_TEST_PA1 =                0x5A;
    static const byte REG_TEST_PA2 =                0x5C;
    static const byte REG_TEST_DAGC =               0x6F;

    static const byte MESSAGE_LENGTH_VARIABLE = 0;
    
    DigitalOut* Pin_RST;            // Reset Pin (1 = Reset)
    DigitalOut* Pin_CS;             // ChipSelect Pin
    DigitalIn* Pin_PayloadReady;    // Gibt an, ob Daten empfangen wurden (GPIO Pin 0)

    byte RegOperationMode;
    byte RegDataModulation;
    word RegBitrate;
    word RegFrequencyDevitation;
    byte RegCarrierFrequency[3];
    byte RegListen[3];
    byte RegPaLevel;
    byte RegRxBw;
    byte RegAfcBw;
    byte RegRssiThresh;
    word RegPreambleSize;
    byte RegSyncConfig;
    byte RegPacketConfig1;
    byte RegPayloadLength;
    byte RegNodeAddress;
    byte RegBroadcastAddress;
    byte RegPacketConfig2;

    bool NeedBoostMode;
    bool NeedPA0;
    short TemperatureCalibrationValue;

    // Konstruktor
public:
    RFM69HCW();

    // Methoden
public:
    // Initialisiert den Chip
    void Begin(DigitalOut* pin_RST, DigitalOut* pin_CS, DigitalIn* pin_DIO0);

    // Muss in jedem Frame aufgerufen werden, um das Empfangen zu ermöglichen
    void Update();
    
    // Chip in den Schlafmodus versetzen
    void EnterSleepMode();

    // Chip in den Bereitschaftsmodus versetzen
    void EnterStandbyMode();

    // Empfangsmodus aktivieren
    void EnterReceiveMode();
    
    // Abhörmodus aktivieren
    void EnterListenMode();

    // Nachricht versenden (variable Länge: 1. Byte = Länge der Nachricht ohne dieses Byte)
    bool TransmitMessage(byte* message);

    // Frequenz und Bitrate konfigurieren
    void ConfigRadio(float mhzFrequency);

    // Nachrichtenlänge konfigurieren
    // variable Länge: 1. Byte = Länge der Nachricht ohne dieses Byte & messageLength = maximale Länge
    // maximal 64 byte Nutzdaten (ohne Längen- und Adressbyte)
    void ConfigMessageLength(bool isLengthFixed, byte messageLength);

    // Zeiten des Abhörmodus konfigurieren
    void ConfigListenMode(Resolution idleTimeRes, byte idleTimeFactor, Resolution rxTimeRes, byte rxTimeFactor);

    // Adressen konfigurieren
    void ConfigAddress(byte nodeAddress, byte groupAddress, AddressFiltering filter);

    // Syncword konfigurieren (BitTolerance = [0;7], 0-8 SyncWord Bytes != 0, bei 0 Bytes wird das SyncWord deaktiviert)
    void ConfigSyncWord(byte toleratedWrongSyncWordBits = 0, byte syncByte1 = 0,
        byte syncByte2 = 0, byte syncByte3 = 0, byte syncByte4 = 0,
        byte syncByte5 = 0, byte syncByte6 = 0, byte syncByte7 = 0, byte syncByte8 = 0);

    // Prüfsumme konfigurieren
    void ConfigCrc(bool isCrcEnabled);

    // Kodierung konfigurieren (bei Manchester-Codierung: BitRate = ChipRate)
    void ConfigEncoding(Encoding code, byte aesKey[16] = 0);

    // Länge der Preamble festlegen (1010...)
    void SetPreambleLength(word length);

    // Frequency Shift Keying aktivieren (Devitation = [0.6;300] =< 500 kHz - BitRate / 2, BitRate = [1.2;300])
    void EnableFskModulation(FskShaping shaping, float kHzFrequencyDevitation, float kbpsBitRate);

    // On-Off-Keying aktivieren (BitRate = [1.2;32768]))
    void EnableOokModulation(OokShaping shaping, float kbpsBitRate);

    // Ausgangsleistung setzen (Power = [-18;20])
    void SetOutputPower(signed char dBmPower);

    // Aktiviert eine hohe Antennenempfindlichkeit
    void SetHighSensitivityMode(bool isEnabled);

    // Eingangsleistung messen
    float MeasureInputPower();

    // Abweichung in MHz zwischen konfigurierter und empfangener Träger-Frequenz messen
    float MeasureFrequencyError();

    // Misst die Umgebungstemperatur [°C] im Standymodus (realTemperature = wahre Temperatur zur Kalibrierung)
    short MeasureTemperature(short realTemperature = 127);

    // Kalibriert den Internen Oszillator im Standbymodus neu (nötig bei großen Temperaturschwankungen ohne Neustart)
    void CalibrateInternalOscillator();

    // Manipulation vom Empfangsfilter
    float SetRxBw(bool inc);
    float SetRxDccFreq(bool inc);
    float SetAfcBw(bool inc);
    float SetAfcDccFreq(bool inc);
    float SetRssiThresh(signed char change);

    // Interruptflags abfragen
    word GetIrqFlags();

private:
    // Ändert den Betriebsmodus des Chip
    void SetMode(Mode mode);

    // Steuert den Antennen-Boost
    void EnableBoostMode(bool isBoosted);
    
    // Bussteuerung
    void SpiBegin();
    void SpiEnd();

    // Sendet bei Änderung (bzw. bei old = 0 immer) den neuen Registerwert und aktualisiert den alten Wert im RAM
    void UpdateRegBit(byte regAddress, bool newBit, byte bit, byte* oldData = 0);
    void UpdateRegBits(byte regAddress, byte newBits, byte bit, byte count, byte* oldData = 0);
    void UpdateRegByte(byte regAddress, byte newByte, byte* oldData = 0);
    void UpdateRegWord(byte regAddress, word newWord, word* oldData = 0);
    void UpdateRegArray(byte regAddress, byte* newData, byte count, byte* oldData = 0);

    // Liest die Daten aus dem angegebenen Register
    byte ReadByte(byte regAddress);
    word ReadWord(byte regAddress);

    // Fifo manipulieren
    void WriteFifo(byte* data, byte count);
    byte ReadFifo(byte* data);
};

#endif
