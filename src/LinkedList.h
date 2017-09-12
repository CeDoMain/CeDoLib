#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Delegate.h"

template<typename T> class LinkedList
{
    // Felder
  private:
      struct Data
      {
        T value;
        Data* Next;
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
      if (First != 0)
      {
        Data* last = First;
        while (last->Next != 0)
          last = last->Next;
        last->Next = new Data { obj, 0 };
      }
      else
      {
        First = new Data { obj, 0 };
      }
    }

    // Führt eine Lambda-Funktion für jedes Element aus
    template<typename TEx, typename ... Ps> void ForEach(TEx expr, Ps ... params)
    {
      Data* data = First;
      while (data != 0)
      {
        expr(data->value, params ...);
        data = data->Next;
      }
    }

    // Gibt das erste Element zurück, für das die Lambda-Funktion wahr ist
    template<typename TEx, typename ... Ps> T Find(TEx expr, Ps ... params)
    {
      Data* data = First;
      while (data != 0)
      {
        if (expr(data->value, params ...))
          return data->value;
        data = data->Next;
      }
      return 0;
    }

    // Zählt die wahren Lambda-Funktionen
    template<typename TEx, typename ... Ps> long Count(TEx expr, Ps ... params)
    {
      long count = 0;
      Data* data = First;
      while (data != 0)
      {
        if (expr(data->value, params ...))
          count++;
        data = data->Next;
      }
      return count;
    }
};

#endif
