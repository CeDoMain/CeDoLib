#ifndef DS3231_H
#define DS3231_H

#include "Arduino.h"
#include "Wire.h"
#include "BaseIO.h"

class DS3231
{
    // Datentypen
public:
    struct DateTime
    {
        unsigned short Year;
        byte Month, Day, Hour, Minute, Second;
    };
    
    // Felder
private:
    static const byte BASE_ADDRESS = 0x68;      // Basisadresse des Chips

    // Konstruktor
public:
    DS3231();

    // Methoden
public:
    // Initialisiert den Chip
    void Begin();

    // Ändert die aktuelle Uhrzeit und Datum
    void SetDateTime(DateTime value);

    // Ließt die aktuelle Uhrzeit und Datum
    DateTime GetDateTime();

private:
    // Gibt an ob die angegebene Normalzeit in der Sommerzeit liegt
    bool IsSummertime(DateTime rawValue);

    // Wird intern verwendet um Sommer- und Winterzeit umzurechnen
    DateTime ToSummertime(DateTime value);
    DateTime ToWintertime(DateTime value);

    // Wird intern verwendet um BCD Zahlen umzuwandeln
    byte FromBcd(byte value);
    byte ToBcd(byte value);
};

#endif
