#include "BiColorLED.h"

const decimal BiColorLED::RG_Red = 10000;
const decimal BiColorLED::RG_Orange = 7500;
const decimal BiColorLED::RG_Yellow = 5000;
const decimal BiColorLED::RG_LightGreen = 2500;
const decimal BiColorLED::RG_Green = 0;
decimal BiColorLED::GlobalIntensity = 10000;

BiColorLED::BiColorLED()
  : LedA(0), LedB(0),
    Intensity(10000), Ratio(RG_Yellow), IsOn(false), PulsePeriodTime(0),
    LastExecutedGlobalIntensity(GlobalIntensity)
{

}

void BiColorLED::Begin(AnalogOut* ledA, AnalogOut* ledB)
{
  LedA = ledA;
  LedB = ledB;
  Off();
}

void BiColorLED::Update()
{
  if(IsOn && (LastExecutedGlobalIntensity != GlobalIntensity || PulsePeriodTime > 0))
    On();
}

void BiColorLED::Off()
{
  IsOn = false;
  (*LedA)(0);
  (*LedB)(0);
}

void BiColorLED::On()
{
  IsOn = true;
  decimal f = (long)Intensity * GlobalIntensity / 10000;
  if (PulsePeriodTime > 0)
  {
    f = (long)f * abs(20000 * (long)(millis() % PulsePeriodTime) / PulsePeriodTime - 10000) / 10000;
  }
  (*LedA)((long)(10000 - Ratio) * f / 100000000.0);
  (*LedB)((long)Ratio * f / 100000000.0);
  LastExecutedGlobalIntensity = GlobalIntensity;
}

void BiColorLED::SetIntensity(decimal intensity)
{
  Intensity = intensity;
  if(IsOn)
    On();
}

void BiColorLED::SetPulsePerSecond(float pulsePerSecond)
{
  if(pulsePerSecond < 0)
    PulsePeriodTime = 0;
  PulsePeriodTime = (float)1000 / pulsePerSecond;
  if (IsOn)
    On();
}

void BiColorLED::SetRatio(decimal ratio)
{
  Ratio = ratio;
  if(IsOn)
    On();
}

void BiColorLED::SetGlobalIntensity(decimal globalIntensity)
{
  GlobalIntensity = globalIntensity;
}
