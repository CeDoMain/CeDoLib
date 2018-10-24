#ifndef BASEIO_H
#define BASEIO_H

#include "Delegate.h"

// Verändert bzw. prüft das y-te Bit in x
#define SET(x, y) (x |= (1 << y))
#define UNSET(x, y) (x &= ~(1 << y))
#define ISSET(x, y) ((x & (1 << y)) != 0)
#define ISUNSET(x, y) ((x & (1 << y)) == 0)

// Wrapper-Schnittstellen für IOs definieren
using AnalogIn = Delegate<float>;
using AnalogOut = Delegate<void, float>;
using DigitalIn = Delegate<bool>;
using DigitalOut = Delegate<void, bool>;

#endif
