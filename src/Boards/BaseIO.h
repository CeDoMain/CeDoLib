#ifndef BASEIO_H
#define BASEIO_H

#include "Delegate.h"

// In x ab dem y-ten Bit, z-Bits durch v ersetzen
#define COPY(x, y, z, v) ((x) ^= ((((x) >> (y)) ^ (v)) & ~(0xFFFFFFFF << (z))) << (y))

// Aus x ab dem y-ten Bit, z-Bits extrahieren
#define EXTRACT(x, y, z) (((x) >> (y)) & ~(0xFFFFFFFF << (z)))

// Verändert bzw. prüft das y-te Bit in x
#define SET(x, y) ((x) |= (1 << (y)))
#define UNSET(x, y) ((x) &= ~(1 << (y)))
#define ISSET(x, y) (((x) & (1 << (y))) != 0)
#define ISUNSET(x, y) (((x) & (1 << (y))) == 0)

// Wrapper-Schnittstellen für IOs definieren
using AnalogIn = Delegate<float>;
using AnalogOut = Delegate<void, float>;
using DigitalIn = Delegate<bool>;
using DigitalOut = Delegate<void, bool>;

#endif
