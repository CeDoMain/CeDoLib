#include "Timer.h"

Timer::Timer(unsigned long intervall)
    : TimeIsUpEvent(0), Intervall(intervall), IsRunning(false)
{
  
}
void Timer::Update()
{
    // Prüfen ob der Timer läuft und abgelaufen ist
    if(IsRunning && millis() - StartTime >= (unsigned long)Intervall)
    {
        // Timer ist abgelaufen
        Stop();

        // Wenn das Event verknüpft ist, auslösen
        if (TimeIsUpEvent != 0)
            (*TimeIsUpEvent)();
    }
}
void Timer::Start()
{
    IsRunning = true;
    StartTime = millis();
}
void Timer::Stop()
{
    IsRunning = false;
}
void Timer::SetTime(unsigned long intervall)
{
    Intervall = intervall;
}
