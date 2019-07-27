#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Delegate.h"

template<typename T> class LinkedList
{
    // Felder
private:
    struct Data
    {
        T value;        // Wert
        Data* Next;     // Verknüpfung zum nächsten Element
    }* First;

    // Konstruktor
public:
    LinkedList()
      : First(0)
    {

    }

    // Methoden
public:
    // Fügt ein Element der Liste hinzu
    void Add(T obj)
    {
        // Prüfen ob es schon einen Anfang gibt
        if (First != 0)
        {
            // Es gibt einen Anfang
            Data* last = First;

            // Letztes Element in der Kette finden
            while (last->Next != 0)
                last = last->Next;

            // Element mit Wert anhängen
            last->Next = new Data { obj, 0 };
        }
        else
        {
            // Es gibt noch keinen Anfang, also wird einer erzeugt
            First = new Data { obj, 0 };
        }
    }

    // Führt eine Lambda-Funktion für jedes Element aus
    template<typename TEx, typename ... Ps> void ForEach(TEx expr)
    {
        Data* data = First;
        while (data != 0)
        {
            // Wert als Parameter übergeben
            expr(data->value);
            data = data->Next;
        }
    }

    // Gibt das erste Element zurück, für das die Lambda-Funktion wahr ist
    template<typename TEx, typename ... Ps> T Find(TEx expr)
    {
        Data* data = First;
        while (data != 0)
        {
            // Lambda-Funktion mit Wert als Parameter ausführen und Rückgabewert prüfen
            if (expr(data->value))
                return data->value;
            data = data->Next;
        }
        return 0;
    }

    // Zählt die wahren Lambda-Funktionen
    template<typename TEx, typename ... Ps> long Count(TEx expr)
    {
        long count = 0;
        Data* data = First;
        while (data != 0)
        {
            // Wenn die Lambda-Funktion für den Parameter wahr ist, Zähler erhöhen
            if (expr(data->value))
                count++;
            data = data->Next;
        }
        return count;
    }
};

#endif
