#ifndef BICOLORLED_H
#define BICOLORLED_H

#include "PCA9685.h"

// Festkommatyp mit 4 Nachkommastellen und Vorzeichen
typedef short decimal;

class BiColorLED
{
    // Felder
private:
    AnalogOut* LedA;                      // Erste LED
    AnalogOut* LedB;                      // Zweite LED
    decimal Intensity;                    // Helligkeit der LEDs
    decimal Ratio;                        // Verhältnis zwischen A (100% = 0) und B (100% = 1)
    bool IsOn;                            // Gibt an, ob die LEDs angeschaltet sind
    unsigned short PulsePeriodTime;       // Periodendauer eines Pulses in ms
    decimal LastExecutedGlobalIntensity;  // Letzter globaler Helligkeitswert, der angewendet wurde
    static decimal GlobalIntensity;       // Globaler Helligkeitswert
public:
    static const decimal RG_Red;          // Verhältnis für Farbe Rot
    static const decimal RG_Orange;       // Verhältnis für Farbe Orange
    static const decimal RG_Yellow;       // Verhältnis für Farbe Gelb
    static const decimal RG_LightGreen;   // Verhältnis für Farbe Hellgrün
    static const decimal RG_Green;        // Verhältnis für Farbe Grün

    // Konstruktor
public:
    BiColorLED();

    // Methoden
public:
    // Initialisiert die LED-Steuerung
    void Begin(AnalogOut* ledA, AnalogOut* ledB);

    // Muss in jedem Frame aufgerufen werden
    void Update();

    // Schaltet die LEDs aus
    void Off();

    // Schaltet die LEDs an
    void On();

    // Ändert die Helligkeit
    void SetIntensity(decimal intensity);

    // Lässt die LEDs pulsieren
    void SetPulsePerSecond(float pulsePerSecond);

    // Ändert das Verhältnis der beiden LEDs
    void SetRatio(decimal ratio);

    // Ändert die globale Helligkeit
    static void SetGlobalIntensity(decimal globalIntensity);
};

#endif
