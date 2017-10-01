#include "SIMKNX128.h"

Event<byte, char*> SIMKNX128::AnyValueRecvEvent = Event<byte, char*>();

SIMKNX128::SIMKNX128()
{
	for (unsigned int i = 0; i < 128 + 10; i++)
		ValueRecvEvent[i] = 0;
}

void SIMKNX128::Begin()
{
	SIMKNX_SERIAL.begin(9600);
}

void SIMKNX128::Update()
{
	char* line = SIMKNX_ReadLine();
	if (line != NULL)
	{
		Serial.println(line);

		// Es wurde ein Befehl empfangen
		char* token = strtok(line, "$ ");

		// Prüfen ob es ein Updatebefehl für eine Gruppenadresse ist
		if (strncmp(token, "oui", 3) != 0)
			return;

		// Kommunikationsobjekt-Index auslesen
		token = strtok(NULL, "$ ");
		int object = (int)strtol(token, NULL, 16);

		// RAM-Flags überspringen
		token = strtok (NULL, "$ ");

		// Neuen Wert auslesen
		token = strtok(NULL, "\0");

		// Event auslösen
		if (ValueRecvEvent != 0)
			(*ValueRecvEvent[object])(token);
		AnyValueRecvEvent(object, token);
	}

	line = USB_ReadLine();
	if (line != NULL)
	{
		// Es wurde ein Befehl empfangen
		char* token = strtok(line, " ");
		byte object = (byte)strtol(token, NULL, 10);
		token = strtok(NULL, " ");
		char* value = strcmp(token, "0") == 0 ? "0 " : "1 ";

		// Event auslösen
		if (ValueRecvEvent != 0)
			(*ValueRecvEvent[object])(token);
		AnyValueRecvEvent(object, value);
	}
}

void SIMKNX128::SendBool(byte object, bool value)
{
	char message[14];
	sprintf(message, "ovs (%i) $%X", object, value);
	SIMKNX_SERIAL.println(message);
}

bool SIMKNX128::ParseBool(char* value)
{
	return strcmp(value, "1 ") == 0;
}

char* SIMKNX128::SIMKNX_ReadLine()
{
	static char Buffer[32];
	static byte i = 0;
	char c;

	// Nachricht einlesen
	while (SIMKNX_SERIAL.available() != 0)
	{
		// Der Index ist 0, wenn im letzten Durchgang eine Zeile komplett eingelesen wurde - der Puffer kann zurückgesetzt werden
		if (i == 0)
			memset(Buffer, 0, sizeof(Buffer));

		// Zeichen einlesen
		c = SIMKNX_SERIAL.read();

		// Das CR (Carriage Return) Zeichen markiert das Zeilenende
		if (c == 13) // CR Zeichen
		{
			Buffer[i] = '\0';
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
	}
	return NULL;
}
char* SIMKNX128::USB_ReadLine()
{
	static char Buffer[32];
	static short i = 0;

	// Wenn es keine Nachrichten gibt kann abgebrochen werden
	if (Serial.available() == 0)
		return NULL;

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
	return NULL;
}
