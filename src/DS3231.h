#ifndef DS3231_H
#define DS3231_H

#include "Arduino.h"
#include "Wire.h"
#include "BaseIO.h"
#include "Timer.h"

class DS3231
{
    // Datentypen
public:
    struct DateTime
    {
        unsigned short Year;
        byte Month, Day, Hour, Minute, Second;
        byte DayOfWeek;     // Von 1 bis 7
        
        DateTime() : Year(2000), Month(1), Day(1), Hour(0), Minute(0), Second(0), DayOfWeek(6) { }
        bool operator!= (DateTime t)
        {
            return Second != t.Second || Minute != t.Minute || Hour != t.Hour
                || Day != t.Day || Month != t.Month || Year != t.Year;
        }
        bool operator== (DateTime t)
        {
            return Second == t.Second && Minute == t.Minute && Hour == t.Hour
                && Day == t.Day && Month == t.Month && Year == t.Year;
        }
    };
    enum Alarm1Rate
    {
        OncePerS =      B01111,
        MatchS =        B01110,
        MatchMS =       B01100,
        MatchHMS =      B01000,
        MatchDateHMS =  B00000,
        MatchDayHMS =   B10000
    };
    enum Alarm2Rate
    {
        OncePerM =      B0111,
        MatchM =        B0110,
        MatchHM =       B0100,
        MatchDateHM =   B0000,
        MatchDayHM =    B1000
    };
    
    // Felder
public:
    Delegate<>* Alarm1Event;
    Delegate<>* Alarm2Event;
    Delegate<>* BatteryLowEvent;

private:
    static const byte BASE_ADDRESS = 0x68;
    static const byte REG7_NOW = 0x00;
    static const byte REG4_ALARM1 = 0x07;
    static const byte REG3_ALARM2 = 0x0B;
    static const byte REG_CONTROL = 0x0E;
    static const byte BIT_ALARM2_INT = 1;
    static const byte BIT_ALARM1_INT = 0;
    static const byte REG_STATUS = 0x0F;
    static const byte BIT_OSCILLATOR_STOP_FLAG = 7;
    static const byte BIT_ALARM2_FLAG = 1;
    static const byte BIT_ALARM1_FLAG = 0;

    Timer BatteryCheckTimer;
    DigitalIn* InterruptPin;

    const bool UseSummertime;

    // Konstruktor
public:
    DS3231(bool useSummertime = true);

    // Methoden
public:
    // Initialisiert den Chip
    void Begin(DigitalIn* interruptPin);

    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Uhrzeit und Datum
    void SetNow(DateTime value);
    DateTime GetNow();

    // Alarme
    void SetAlarm1(DateTime value, Alarm1Rate rate);
    void SetAlarm2(DateTime value, Alarm2Rate rate);
    void SetEnableAlarm1(bool isEnabled);
    void SetEnableAlarm2(bool isEnabled);
    void GetAlarm1(DateTime* value, Alarm1Rate* rate = 0, bool* isEnabled = 0);
    void GetAlarm2(DateTime* value, Alarm2Rate* rate = 0, bool* isEnabled = 0);

private:
    // Gibt an ob die angegebene Normalzeit in der Sommerzeit liegt
    bool IsSummertime(DateTime rawValue);

    // Rechnet Sommer- und Winterzeit um
    DateTime ToSummertime(DateTime value);
    DateTime ToWintertime(DateTime value);

    // Wandelt BCD Zahlen um
    byte FromBcd(byte value);
    byte ToBcd(byte value);

    // Daten senden und empfangen
    void WriteRegByte(byte regAddress, byte data);
    void WriteRegArray(byte regAddress, byte* data, byte count);
    byte ReadRegByte(byte regAddress);
    void ReadRegArray(byte regAddress, byte* data, byte count);
};

#endif
