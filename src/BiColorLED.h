#ifndef BICOLORLED_H
#define BICOLORLED_H

#include "PCA9685.h"

class BiColorLED
{
  // Felder
private:
  AnalogOut* LedA;                    // Erste LED
  AnalogOut* LedB;                    // Zweite LED
  float Intensity;                    // Helligkeit der LEDs
  float Ratio;                        // Verhältnis zwischen A (0) und B (1)
  bool IsOn;                          // Gibt an, ob die LEDs angeschaltet sind
  float PulsePerSecond;               // Pulse pro Sekunde der LEDs
  float LastExecutedGlobalIntensity;  // Letzter globaler Helligkeitswert, der angewendet wurde
  static float GlobalIntensity;       // Globaler Helligkeitswert
public:
  static const float RG_Red;
  static const float RG_Orange;
  static const float RG_Yellow;
  static const float RG_LightGreen;
  static const float RG_Green;

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
  void SetIntensity(const float intensity);

  // Lässt die LEDs pulsieren
  void SetPulsePerSecond(const float pulsePerSecond);

  // Ändert das Verhältnis der beiden LEDs
  void SetRatio(const float ratio);

  static void SetGlobalIntensity(const float globalIntensity);
};

#endif
