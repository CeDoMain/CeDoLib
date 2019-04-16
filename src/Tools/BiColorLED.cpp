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
    // Ausgangspins verknüpfen und ausschalten
    LedA = ledA;
    LedB = ledB;
    Off();
}

void BiColorLED::Update()
{
    // Wenn die LED an ist und gedimmt werden muss, Ausgänge neuberechnen
    if(IsOn && (LastExecutedGlobalIntensity != GlobalIntensity || PulsePeriodTime > 0))
        On();
}

void BiColorLED::Off()
{
    // Ausgangspins ausschalten
    IsOn = false;
    (*LedA)(0);
    (*LedB)(0);
}

void BiColorLED::On()
{
    // Helligkeit berechnen
    IsOn = true;
    decimal f = (long)Intensity * GlobalIntensity / 10000;

    // Prüfen ob die LED pulsieren soll
    if (PulsePeriodTime > 0)
    {
        // Dreieckspuls generieren und aktuellen Wert berechnen
        f = (long)f * abs(20000 * (long)(millis() % PulsePeriodTime) / PulsePeriodTime - 10000) / 10000;
    }
    // Ausgangspins entsprechend der Helligkeit und Farbe einstellen
    (*LedA)((long)(10000 - Ratio) * f / 100000000.0);
    (*LedB)((long)Ratio * f / 100000000.0);

    // globale Helligkeit wurde eingerechnet
    LastExecutedGlobalIntensity = GlobalIntensity;
}

void BiColorLED::SetIntensity(decimal intensity)
{
    // Helligkeit speichern und Ausgänge neuberechnen
    Intensity = intensity;
    if(IsOn)
        On();
}

void BiColorLED::SetPulsePerSecond(float pulsePerSecond)
{
    // Pulsieren (de)aktivieren und Pulszeit berechnen
    if(pulsePerSecond < 0)
        PulsePeriodTime = 0;
    PulsePeriodTime = (float)1000 / pulsePerSecond;

    // Wenn die LED an ist, Ausgänge neuberechnen
    if (IsOn)
        On();
}

void BiColorLED::SetRatio(decimal ratio)
{
    // Farbe speichern und Ausgänge neuberechnen
    Ratio = ratio;
    if(IsOn)
        On();
}

void BiColorLED::SetGlobalIntensity(decimal globalIntensity)
{
    // Globale Helligkeit einstellen
    GlobalIntensity = globalIntensity;
}
