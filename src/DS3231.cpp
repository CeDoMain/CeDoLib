#include "DS3231.h"

DS3231::DS3231(bool useSummertime)
    : Alarm1Event(0), Alarm2Event(0), BatteryLowEvent(0),
      BatteryCheckTimer(60000), InterruptPin(0), UseSummertime(useSummertime)
{
    
}
void DS3231::Begin(DigitalIn* interruptPin)
{
    Wire.begin();
    InterruptPin = interruptPin;

    // RTC Konfigurieren
    byte buffer[2];
    ReadRegArray(REG_CONTROL, buffer, 2);
    COPY(buffer[0], 2, 6, B000001);
    COPY(buffer[1], 0, 4, B0000);
    WriteRegArray(REG_CONTROL, buffer, 2);
    
    BatteryCheckTimer.TimeIsUpEvent = new Delegate<>([&]()
    {
        byte data = ReadRegByte(REG_STATUS);
        if (BatteryLowEvent != 0 && ISSET(data, BIT_OSCILLATOR_STOP_FLAG))
        {
            (*BatteryLowEvent)();
            UNSET(data, BIT_OSCILLATOR_STOP_FLAG);
            WriteRegByte(REG_STATUS, data);
        }
        BatteryCheckTimer.Start();
    });

    BatteryCheckTimer.Start();
}
void DS3231::Update()
{
    if (!(*InterruptPin)())
    {
        byte data[2];
        ReadRegArray(REG_CONTROL, data, 2);
        if (Alarm1Event != 0 && ISSET(data[0], BIT_ALARM1_INT) && ISSET(data[1], BIT_ALARM1_FLAG))
            (*Alarm1Event)();
        if (Alarm2Event != 0 && ISSET(data[0], BIT_ALARM2_INT) && ISSET(data[1], BIT_ALARM2_FLAG))
            (*Alarm2Event)();

        COPY(data[1], 0, 2, B00);
        WriteRegByte(REG_STATUS, data[1]);
    }
    BatteryCheckTimer.Update();
}
void DS3231::SetNow(DateTime value)
{
    // Auf Sommerzeit prüfen
    if (IsSummertime(value))
        value = ToWintertime(value);

    // Zeit umwandeln (24 Stunden Format)
    byte buffer[7] =
    {
        ToBcd(constrain(value.Second, 0, 59)),
        ToBcd(constrain(value.Minute, 0, 59)),
        ToBcd(constrain(value.Hour, 0, 23)),
        constrain(value.DayOfWeek, 1, 7),
        ToBcd(constrain(value.Day, 1, 31)),
        ToBcd(constrain(value.Month, 1, 12)),
        ToBcd(constrain(value.Year - 2000, 0, 99))
    };
    
    // Zeit hochladen
    WriteRegArray(REG7_NOW, buffer, 7);
}
DS3231::DateTime DS3231::GetNow()
{
    // Zeit auslesen
    byte buffer[7];
    ReadRegArray(REG7_NOW, buffer, 7);

    // Zeit umwandeln (24 Stunden Format)
    DateTime now;
    now.Second = FromBcd(buffer[0]);
    now.Minute = FromBcd(buffer[1]);
    now.Hour = FromBcd(EXTRACT(buffer[2], 0, 6));
    now.DayOfWeek = buffer[3];
    now.Day = FromBcd(buffer[4]);
    now.Month = FromBcd(EXTRACT(buffer[5], 0, 5));
    now.Year = 2000 + FromBcd(buffer[6]);

    return IsSummertime(now) ? ToSummertime(now) : now;
}
void DS3231::SetAlarm1(DateTime value, Alarm1Rate rate)
{
    // Auf Sommerzeit prüfen
    if (IsSummertime(value))
        value = ToWintertime(value);

    // Zeit umwandeln (24 Stunden Format)
    byte buffer[4] =
    {
        ToBcd(constrain(value.Second, 0, 59)),
        ToBcd(constrain(value.Minute, 0, 59)),
        ToBcd(constrain(value.Hour, 0, 23)),
        rate == Alarm1Rate::MatchDayHMS
            ? constrain(value.DayOfWeek, 1, 7) + 0x40
            : ToBcd(constrain(value.Day, 1, 31))
    };

    // Flags setzen
    for (int i = 0; i < 4; i++)
    {
        if (ISSET(rate, i))
            SET(buffer[i], 7);
    }
    
    // Konfiguration hochladen
    WriteRegArray(REG4_ALARM1, buffer, 4);
}
void DS3231::SetAlarm2(DateTime value, Alarm2Rate rate)
{
    // Auf Sommerzeit prüfen
    if (IsSummertime(value))
        value = ToWintertime(value);

    // Zeit umwandeln (24 Stunden Format)
    byte buffer[3] =
    {
        ToBcd(constrain(value.Minute, 0, 59)),
        ToBcd(constrain(value.Hour, 0, 23)),
        rate == Alarm2Rate::MatchDayHM
            ? constrain(value.DayOfWeek, 1, 7) + 0x40
            : ToBcd(constrain(value.Day, 1, 31))
    };

    // Flags setzen
    for (int i = 0; i < 3; i++)
    {
        if (ISSET(rate, i))
            SET(buffer[i], 7);
    }
    
    // Konfiguration hochladen
    WriteRegArray(REG3_ALARM2, buffer, 3);
}
void DS3231::SetEnableAlarm1(bool isEnabled)
{
    byte data = ReadRegByte(REG_CONTROL);
    if (isEnabled != ISSET(data, BIT_ALARM1_INT))
    {
        isEnabled ? SET(data, BIT_ALARM1_INT) : UNSET(data, BIT_ALARM1_INT);
        WriteRegByte(REG_CONTROL, data);
    }
}
void DS3231::SetEnableAlarm2(bool isEnabled)
{
    byte data = ReadRegByte(REG_CONTROL);
    if (isEnabled != ISSET(data, BIT_ALARM2_INT))
    {
        isEnabled ? SET(data, BIT_ALARM2_INT) : UNSET(data, BIT_ALARM2_INT);
        WriteRegByte(REG_CONTROL, data);
    }
}
void DS3231::GetAlarm1(DateTime* value, Alarm1Rate* rate, bool* isEnabled)
{
    // Daten auslesen
    byte buffer[6];
    ReadRegArray(REG4_ALARM1, buffer, 4);
    buffer[4] = ReadRegByte(REG_CONTROL);
    buffer[5] = ReadRegByte(REG_STATUS);

    if (value != 0)
    {
        // Zeit umwandeln (24 Stunden Format)
        DateTime time;
        time.Second = FromBcd(EXTRACT(buffer[0], 0, 7));
        time.Minute = FromBcd(EXTRACT(buffer[1], 0, 7));
        time.Hour = FromBcd(EXTRACT(buffer[2], 0, 6));
        (ISSET(buffer[3], 6) ? time.DayOfWeek : time.Day) = FromBcd(EXTRACT(buffer[3], 0, 6));
        *value = IsSummertime(time) ? ToSummertime(time) : time;
    }
    if (rate != 0)
    {
        // Flags prüfen
        byte rawRate = 0x00;
        for (int i = 0; i < 4; i++)
        {
            if (ISSET(buffer[i], 7))
                SET(rawRate, i);
        }
        if (ISSET(buffer[3], 6))
            SET(rawRate, 4);
        *rate = (Alarm1Rate)rawRate;
    }
    if (isEnabled != 0)
        *isEnabled = ISSET(buffer[4], BIT_ALARM1_INT);
}
void DS3231::GetAlarm2(DateTime* value, Alarm2Rate* rate, bool* isEnabled)
{
    // Daten auslesen
    byte buffer[4];
    ReadRegArray(REG3_ALARM2, buffer, 4);

    if (value != 0)
    {
        // Zeit umwandeln (24 Stunden Format)
        DateTime time;
        time.Minute = FromBcd(EXTRACT(buffer[0], 0, 7));
        time.Hour = FromBcd(EXTRACT(buffer[1], 0, 6));
        (ISSET(buffer[2], 6) ? time.DayOfWeek : time.Day) = FromBcd(EXTRACT(buffer[2], 0, 6));
        *value = IsSummertime(time) ? ToSummertime(time) : time;
    }
    if (rate != 0)
    {
        // Flags prüfen
        byte rawRate = 0x00;
        for (int i = 0; i < 3; i++)
        {
            if (ISSET(buffer[i], 7))
                SET(rawRate, i);
        }
        if (ISSET(buffer[2], 6))
            SET(rawRate, 3);
        *rate = (Alarm2Rate)rawRate;
    }
    if (isEnabled != 0)
        *isEnabled = ISSET(buffer[3], BIT_ALARM2_INT);
}
bool DS3231::IsSummertime(DateTime rawValue)
{
    if (!UseSummertime)
        return false;
        
    byte lastSundayOfMar = 31 - (5 + rawValue.Year + rawValue.Year / 4) % 7;
    byte lastSundayOfOct = 31 - (2 + rawValue.Year + rawValue.Year / 4) % 7;
    return (rawValue.Month > 3 && rawValue.Month < 10)                                          // zwischen März und Oktober
        || (rawValue.Month == 3 && rawValue.Day > lastSundayOfMar)                              // oder nach dem letzten Sonntag im März
        || (rawValue.Month == 10 && rawValue.Day < lastSundayOfOct)                             // oder vor dem letzten Sonntag im Oktober
        || (rawValue.Month == 3 && rawValue.Day == lastSundayOfMar && rawValue.Hour >= 2)       // oder am letzten Sonntag im März nach 2:00
        || (rawValue.Month == 10 && rawValue.Day == lastSundayOfOct && rawValue.Hour < 2);      // oder am letzten Sonntag im Oktober vor 2:00
}
DS3231::DateTime DS3231::ToSummertime(DateTime value)
{
    // +1 Stunde
    value.Hour++;
    if (value.Hour == 24)
    {
        // nächster Tag
        value.Hour = 0;
        value.Day++;
        value.DayOfWeek += value.DayOfWeek == 7 ? -6 : 1;
    }
    // verschiedene Monatslängen zwischen März und Oktober beachten
    switch (value.Month)
    {
        case 4:
        case 6:
        case 9:
            if (value.Day == 31)
            {
                value.Month++;
                value.Day = 1;
            }
            break;
        default:
            if (value.Day == 32)
            {
                value.Month++;
                value.Day = 1;
            }
            break;
    }
    return value;
}
DS3231::DateTime DS3231::ToWintertime(DateTime value)
{
    // -1 Stunde
    if (value.Hour == 0)
    {
        // vorheriger Tag
        value.Hour = 23;
        value.Day--;
        value.DayOfWeek -= value.DayOfWeek == 1 ? -6 : 1;
    }
    else
    {
        value.Hour--;
    }
    // verschiedene Monatslängen zwischen März und Oktober beachten
    if (value.Day == 0)
    {
        value.Month--;
        switch (value.Month)
        {
            case 4:
            case 6:
            case 9:
                value.Day = 30;
                break;
            default:
                value.Day = 31;
                break;
        }
    }
    return value;
}
byte DS3231::FromBcd(byte value)
{
    return EXTRACT(value, 4, 4) * 10 + EXTRACT(value, 0, 4);
}
byte DS3231::ToBcd(byte value)
{
    return ((value / 10) << 4) + (value % 10);
}
void DS3231::WriteRegByte(byte regAddress, byte data)
{
	// 1 Byte in angegebenes Register schreiben
	Wire.beginTransmission(BASE_ADDRESS);
	Wire.write(regAddress);
	Wire.write(data);
	Wire.endTransmission();
}
void DS3231::WriteRegArray(byte regAddress, byte* data, byte count)
{
	// Mehrere Bytes in angegebenes Register schreiben
	Wire.beginTransmission(BASE_ADDRESS);
	Wire.write(regAddress);
    for (int i = 0; i < count; i++)
    {
        Wire.write(data[i]);
    }
	Wire.endTransmission();
}
byte DS3231::ReadRegByte(byte regAddress)
{
	// Byte aus angegebenem Register anfordern
	Wire.beginTransmission(BASE_ADDRESS);
	Wire.write(regAddress);
	Wire.endTransmission(false);
	Wire.requestFrom((int)BASE_ADDRESS, (int)1);

	// Byte einlesen
    return Wire.available() ? Wire.read() : 0x00;
}
void DS3231::ReadRegArray(byte regAddress, byte* data, byte count)
{
	// Mehrere Bytes aus angegebenem Register anfordern
	Wire.beginTransmission(BASE_ADDRESS);
	Wire.write(regAddress);
	Wire.endTransmission(false);
	Wire.requestFrom((int)BASE_ADDRESS, (int)count);

	// Bytes einlesen
    for (int i = 0; i < count; i++)
	{
        data[i] = Wire.available() ? Wire.read() : 0x00;
	}
}
