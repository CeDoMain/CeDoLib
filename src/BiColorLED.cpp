#include "BiColorLED.h"

const decimal BiColorLED::RG_Red = 10000;
const decimal BiColorLED::RG_Orange = 7500;
const decimal BiColorLED::RG_Yellow = 5000;
const decimal BiColorLED::RG_LightGreen = 2500;
const decimal BiColorLED::RG_Green = 0;
decimal BiColorLED::GlobalIntensity = 10000;

BiColorLED::BiColorLED()
  : LedA(0), LedB(0),
    Intensity(10000), Ratio(RG_Yellow), IsOn(false), PulsePerSecond(0),
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
  if(IsOn && (LastExecutedGlobalIntensity != GlobalIntensity || PulsePerSecond != 0))
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
  if (PulsePerSecond != 0)
  {
    unsigned long T = 10000000 / PulsePerSecond;
    f = f * abs(20000 * (millis() % T) / T - 10000) / 10000;
  }
  (*LedA)((1 - Ratio) * f / 10000.0);
  (*LedB)(Ratio * f / 10000.0);
  LastExecutedGlobalIntensity = GlobalIntensity;
}

void BiColorLED::SetIntensity(decimal intensity)
{
  Intensity = constrain(intensity, 0, 1);
  if(IsOn)
    On();
}

void BiColorLED::SetPulsePerSecond(decimal pulsePerSecond)
{
  if(pulsePerSecond < 0)
  {
    PulsePerSecond = 0;
    return;
  }
  PulsePerSecond = pulsePerSecond;
  if (IsOn)
    On();
}

void BiColorLED::SetRatio(decimal ratio)
{
  Ratio = constrain(ratio, 0, 1);
  if(IsOn)
    On();
}

void BiColorLED::SetGlobalIntensity(decimal globalIntensity)
{
  GlobalIntensity = globalIntensity;
}
