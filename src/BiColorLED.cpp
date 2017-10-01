#include "BiColorLED.h"

const float BiColorLED::RG_Red = 1;
const float BiColorLED::RG_Orange = 0.75;
const float BiColorLED::RG_Yellow = 0.5;
const float BiColorLED::RG_LightGreen = 0.25;
const float BiColorLED::RG_Green = 0;
float BiColorLED::GlobalIntensity = 1;

BiColorLED::BiColorLED()
  : LedA(0), LedB(0),
    Intensity(1), Ratio(0.5), IsOn(false), PulsePerSecond(0),
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
  float f = Intensity * GlobalIntensity;
  if (PulsePerSecond != 0)
  {
    unsigned short T = 1000 / PulsePerSecond;
    f *= abs(2 * (millis() % T) / T - 1);
  }
  (*LedA)((1 - Ratio) * f);
  (*LedB)(Ratio * f);
  LastExecutedGlobalIntensity = GlobalIntensity;
}

void BiColorLED::SetIntensity(const float intensity)
{
  Intensity = constrain(intensity, 0, 1);
  if(IsOn)
    On();
}

void BiColorLED::SetPulsePerSecond(const float pulsePerSecond)
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

void BiColorLED::SetRatio(const float ratio)
{
  Ratio = constrain(ratio, 0, 1);
  if(IsOn)
    On();
}

void BiColorLED::SetGlobalIntensity(const float globalIntensity)
{
  GlobalIntensity = globalIntensity;
}
