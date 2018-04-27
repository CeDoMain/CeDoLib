#ifndef BASEIO_H
#define BASEIO_H

#include "Delegate.h"

// Wrapper-Schnittstellen für IOs definieren
using AnalogIn = Delegate<float>;
using AnalogOut = Delegate<void, float>;
using DigitalIn = Delegate<bool>;
using DigitalOut = Delegate<void, bool>;

#endif
