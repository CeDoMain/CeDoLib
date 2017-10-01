#include "Timer.h"

Timer::Timer(unsigned int intervall)
    : TimeIsUpEvent(0), Intervall(intervall), IsRunning(false)
{
  
}
void Timer::Update()
{
    if(IsRunning && millis() - StartTime >= (unsigned long)Intervall)
    {
        if (TimeIsUpEvent != 0)
            (*TimeIsUpEvent)();
        Stop();
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
void Timer::SetTime(unsigned int intervall)
{
    Intervall = intervall;
    Start();
}
