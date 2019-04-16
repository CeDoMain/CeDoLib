#ifndef EVENT_H
#define EVENT_H

#include "Delegate.h"

template<typename ... Ps> class Event : public Delegate<void, Ps ...>
{
    // Felder
private:
    struct Data
    {
        Delegate<void, Ps ...> call;
        Data* Next;
    }* First;

public:
    // Konstruktor
    Event()
        : First(0)
    {

    }

    // Operatoren
public:
    // Löst das Event aus
    void operator()(Ps ... params) override
    {
        Data* data = First;
        while (data != 0)
        {
            data->call(params ...);
            data = data->Next;
        }
    }

    // Methoden
public:
    // Verknüpft das Event mit einer Memberfunktion
    template<typename T> void Connect(T* obj, void (T::*fp)(Ps ...))
    {
        if (First != 0)
        {
            Data* last = First;
            while (last->Next != 0)
                last = last->Next;
            last->Next = new Data { Delegate<void, Ps ...>(obj, fp), 0 };
        }
        else
        {
            First = new Data { Delegate<void, Ps ...>(obj, fp), 0 };
        }
    }
    // Verknüpft das Event mit einer statischen Funktion
    void Connect(void (*fp)(Ps ...))
    {
        if (First != 0)
        {
            Data* last = First;
            while (last->Next != 0)
                last = last->Next;
            last->Next = new Data { Delegate<void, Ps ...>(fp), 0 };
        }
        else
        {
            First = new Data { Delegate<void, Ps ...>(fp), 0 };
        }
    }
    // Verknüpft das Event mit einer Lambdafunktion
    template<typename T> void Connect(T expr)
    {
        if (First != 0)
        {
            Data* last = First;
            while (last->Next != 0)
                last = last->Next;
            last->Next = new Data { Delegate<void, Ps ...>(expr), 0 };
        }
        else
        {
            First = new Data { Delegate<void, Ps ...>(expr), 0 };
        }
    }
};

#endif
