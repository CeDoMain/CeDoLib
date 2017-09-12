#ifndef SIMKNX128_H
#define SIMKNX128_H

#include "Arduino.h"
#include "Event.h"

#define SIMKNX_SERIAL Serial1
#define DEBUG 1

class SIMKNX128
{
    // Felder
  public:
    Event<char*> ValueRecvEvent[128];             // Array von Events, die bei Empfang ausgelöst werden
    static Event<byte, char*> AnyValueRecvEvent;

    // Konstruktor
    SIMKNX128();

    // Methoden
  public:
    // Startet die Verbindung zum SIM-KNX-Modul
    void Begin();

    // Muss in jedem Frame der Loop aufgerufen werden
    void Update();

    // Sendet einen Wahrheitswert an das angegebene Kommunikationsobjekt
    void SendBool(byte object, bool value);

    // Prüft ob die Rückmeldung wahr oder Falsch ist
    static bool ParseBool(char*);

  private:
    // Liest bei jedem Aufruf ein Zeichen von SIMKNX_SERIAL und gibt den String bei Zeilenende zurück
    static char* SIMKNX_ReadLine();

    // Liest bei jedem Aufruf ein Zeichen von USB_SERIAL und gibt den String bei Zeilenende zurück
    static char* USB_ReadLine();
};

#endif
