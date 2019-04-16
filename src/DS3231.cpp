#include "DS3231.h"

DS3231::DS3231()
{

}

void DS3231::Begin()
{
    Wire.begin();
}

void DS3231::SetDateTime(DateTime value)
{
    // Auf Sommerzeit prüfen
    if (IsSummertime(value))
        value = ToWintertime(value);

    // Zeit umwandeln (24 Stunden Format)
    byte buffer[8] =
    {
        0x00,
        ToBcd(constrain(value.Second, 0, 59)),
        ToBcd(constrain(value.Minute, 0, 59)),
        ToBcd(constrain(value.Hour, 0, 23)),
        0x00,
        ToBcd(constrain(value.Day, 1, 31)),
        ToBcd(constrain(value.Month, 1, 12)),
        ToBcd(constrain(value.Year - 2000, 0, 99)),
    };
    
    // Zeit hochladen
	Wire.beginTransmission(BASE_ADDRESS);
    for (byte i = 0; i < sizeof(buffer); i++)
    {
        Serial.print(buffer[i], HEX);
        Serial.print(' ');
        Wire.write(buffer[i]);
    }
    Serial.println();
	Wire.endTransmission();
}

DS3231::DateTime DS3231::GetDateTime()
{
    // Zeit auslesen
    byte buffer[7];
	Wire.beginTransmission(BASE_ADDRESS);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(BASE_ADDRESS, 7);
    for (byte i = 0; i < sizeof(buffer); i++)
        buffer[i] = Wire.read();

    // Zeit umwandeln
    DateTime now;
    now.Second = FromBcd(buffer[0]);
    now.Minute = FromBcd(buffer[1]);
    now.Hour = FromBcd(EXTRACT(buffer[2], 0, 5));
    // 12/24 Stunden verarbeiten
    if (ISSET(buffer[2], 5))
        now.Hour += ISSET(buffer[2], 6) ? 12 : 20;
    now.Day = FromBcd(buffer[4]);
    now.Month = FromBcd(EXTRACT(buffer[5], 0, 5));
    now.Year = 2000 + FromBcd(buffer[6]);

    return IsSummertime(now) ? ToSummertime(now) : now;
}

bool DS3231::IsSummertime(DateTime rawValue)
{
    byte lastSundayOfMar = 31 - (5 + rawValue.Year + rawValue.Year / 4) % 7;
    byte lastSundayOfOct = 31 - (2 + rawValue.Year + rawValue.Year / 4) % 7;
    return (rawValue.Month > 3 && rawValue.Month < 10)                                          // zwischen März und Oktober
        || (rawValue.Month == 3 && rawValue.Day > lastSundayOfMar)                              // oder nach dem letzten Sonntag im März
        || (rawValue.Month == 10 && rawValue.Day < lastSundayOfOct)                             // oder vor dem letzten Sonntag im Oktober
        || (rawValue.Month == 3 && rawValue.Day == lastSundayOfMar && rawValue.Hour >= 2)                // oder am letzten Sonntag im März nach 2:00
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