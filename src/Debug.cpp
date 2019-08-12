#include "Debug.h"

LinkedList<SerialToValue::Data*> SerialToValue::Values = LinkedList<SerialToValue::Data*>();

long SerialToValue::GetValue(char key)
{
    Data* d = Values.Find([key](Data* d)
    {
        return d->Key == key;
    });
    if (d != 0)
    {
        // Eintrag vorhanden
        return d->Value;
    }
    else
    {
        // Eintrag anlegen
        CreateValue(key, 0);
        return 0;
    }
}
void SerialToValue::SetValue(char key, long value)
{
    Data* d = Values.Find([key](Data* d)
    {
        return d->Key == key;
    });
    if (d != 0)
    {
        if (d->Value == value)
            return;

        // Eintrag vorhanden
        d->Value = value;

        // Event auslösen
        if (d->ValueChangedEvent != 0)
            (*d->ValueChangedEvent)();
    }
    else
    {
        // Eintrag anlegen
        CreateValue(key, value);
    }
}
void SerialToValue::CreateValue(char key, long value, Delegate<>* event)
{
    Data* d = Values.Find([key](Data* d)
    {
        return d->Key == key;
    });
    if (d != 0)
    {
        // Eintrag vorhanden
        d->Value = value;
        d->ValueChangedEvent = event;
    }
    else
    {
        // Wert anlegen
        Values.Add(new Data(key, value, event));
    }
}
void SerialToValue::Update()
{
	// Nachrichten über den Serial Port einlesen
	char* line = SerialReadLineAsync();
	if (line != 0)
	{
        // Befehl einlesen
		char* token = strtok(line, " ");

        if (strcmp(token, "dump") == 0)
        {
            // Alle Werte ausgeben
            Serial.println("---- Werte ----");
            Values.ForEach([](Data* d)
            {
                Serial.print(d->Key);
                Serial.print(' ');
                Serial.println(d->Value);
            });
            Serial.println("---------------");
            return;
        }

		// Es wurde ein Wert empfangen (Format: "Schlüssel Wert" - Beispiel: A 815)
		char key = token[0];
		token = strtok(0, " ");
		long value = strtol(token, 0, 10);

        SetValue(key, value);
	}
}
char* SerialToValue::SerialReadLineAsync()
{
    static char Buffer[32];
	static short i = 0;

	// Wenn es keine Nachrichten gibt kann abgebrochen werden
	if (Serial.available() == 0)
		return 0;

	// Der Index ist 0, wenn im letzten Durchgang eine Zeile komplett eingelesen wurde - der Puffer kann zurückgesetzt werden
	if (i == 0)
		memset(Buffer, 0, sizeof(Buffer));

	// Zeichen einlesen
	char c = Serial.read();

	// Das CR (Carriage Return) Zeichen markiert das Zeilenende
	if (c == 13) // CR Zeichen
	{
		i = 0;
		return Buffer;
	}
	if (!isControl(c))
	{
		// Zeichen dem Puffer hinzufügen
		Buffer[i] = c;

		// Wenn der Puffer voll ist, soll er ausgegeben werden
		if (i == sizeof(Buffer) - 2)
		{
			i = 0;
			return Buffer;
		}
		i++;
	}
	return 0;
}

void Chrono(char prefix)
{
    static unsigned long lastStamp = 0;
    unsigned long thisStamp = millis();
    Serial.print(prefix);
    Serial.print(' ');
    Serial.println(thisStamp - lastStamp);
    lastStamp = thisStamp;
}

// nachfolgender Code von http://playground.arduino.cc/Main/Printf

void Debug(char* fmt, ... )
{
    // PrintF mit Formatstring aus dem RAM und Ausgabe im Serial Port
    char buf[128];
    va_list args;
    va_start(args, fmt );
    vsnprintf(buf, 128, fmt, args);
    va_end(args);
    Serial.print(buf);
}

#ifdef ARDUINO_AVR_MEGA2560

void Debug(const __FlashStringHelper* fmt, ... )
{
    // PrintF mit Formatstring aus dem Flash und Ausgabe im Serial Port
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(buf, sizeof(buf), (char*)fmt, args);
    va_end(args);
    Serial.print(buf);
}

void PrintRAMUsage()
{
    // Schickt den Anteil des belegten RAMs mit zwei Nachkommastellen im Serial Port an
    double mem = 100.0 - freeMemory() / 8186.0 * 100.0;
    Debug(F("%i.%i%% RAM used\n"), (int)mem, (int)((mem - (int)mem) * 100));
}

#endif