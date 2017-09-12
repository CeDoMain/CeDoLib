#include "Debug.h"

// Code von http://playground.arduino.cc/Main/Printf

void Debug(char* fmt, ... )
{
  char buf[128];
  va_list args;
  va_start(args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end(args);
  Serial.print(buf);
}

void Debug(const __FlashStringHelper* fmt, ... )
{
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf_P(buf, sizeof(buf), (char*)fmt, args);
  va_end(args);
  Serial.print(buf);
}

void PrintRAMUsage()
{
  double mem = 100.0 - freeMemory() / 8186.0 * 100.0;
  Debug(F("%i.%i%% RAM used\n"), (int)mem, (int)((mem - (int)mem) * 100));
}
