#include "Timer.h"

Timer::Timer(unsigned int intervall, bool isEndless, bool isRunning)
{
  this->intervall = intervall;
  this->isEndless = isEndless;
  this->isRunning = isRunning;
}
void Timer::Update()
{
  if(isRunning && millis() - startTime >= (unsigned long)intervall)
  {
    TimeIsUpEvent();
    if (isEndless)
      Start();
    else
      Stop();
  }
}
void Timer::Start()
{
  isRunning = true;
  startTime = millis();
}
void Timer::Stop()
{
  isRunning = false;
}
void Timer::SetTime(unsigned int intervall)
{
  this->intervall = intervall;
  Start();
}
