#include "Trigger.h"

Event<> Trigger::AnyActiveEvent = Event<>();

Trigger::Trigger(DigitalIn* pin, const bool isInverted, const unsigned int dwellTime)
  : Pin(pin), IsInverted(isInverted), LastCheckedValue(false), LastMeasuredValue(false),
    LastMeasurementTimeStamp(0), DwellTime(dwellTime)
{

}

void Trigger::Update()
{
  bool MeasuredValue = (*Pin)() != IsInverted;
  if (MeasuredValue != LastCheckedValue)
  {
    // Wert hat sich gegenüber dem registrierten Zustand geändert
    if (LastMeasurementTimeStamp == 0)
    {
      // Es wurde noch keine Vergleichsmessung gemacht
      LastMeasuredValue = MeasuredValue;
      LastMeasurementTimeStamp = millis();
    }
    else
    {
      // Es wurde eine Vergleichsmessung durchgeführt
      if (millis() - LastMeasurementTimeStamp >= DwellTime)
      {
        // Genügend Zeit für einen Vergleich vergangen
        if (LastMeasuredValue == MeasuredValue)
        {
          // Die Werte haben sich nicht verändert > neuen Wert registrieren
          LastCheckedValue = MeasuredValue;

          // Trigger auslösen
          if (LastCheckedValue)
          {
            Activated();
            AnyActiveEvent();
          }
          else
            Deactivated();
        }
      }
    }
  }
  else
  {
    // Wert hat sich nicht geändert
    LastMeasurementTimeStamp = 0;
  }
}