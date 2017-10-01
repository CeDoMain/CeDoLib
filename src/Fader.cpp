#include "Fader.h"

Fader::Fader()
  : FadeEvent(0), IsFading(false), From(0), To(1), Value(0),
    FadeTime(0), FadeStartTime(0)
{

}

void Fader::Update()
{
  if (!IsFading)
    return;

  // Erzeugt einen sinoiden Verlauf von 1 nach 0
  float f = 0.5 + 0.5 * cos((millis() - FadeStartTime) / 1000.0 * 3.14 / FadeTime);
  if (millis() - FadeStartTime > FadeTime * 1000.0)
  {
    f = 0;
    IsFading = false;
  }
  Value = f * From + (1 - f) * To;
  if (FadeEvent != 0)
    (*FadeEvent)(Value);
}

void Fader::Start(float fadeTime)
{
  Start(fadeTime, 0, 1);
}

void Fader::Start(float fadeTime, float to)
{
  Start(fadeTime, Value, to);
}

void Fader::Start(float fadeTime, float from, float to)
{
  if(from == to)
  {
    if(Value != To)
    {
      Value = To;
      if (FadeEvent != 0)
        (*FadeEvent)(To);
    }
    return;
  }
  FadeTime = fadeTime;
  From = from;
  To = to;
  FadeStartTime = millis();
  IsFading = true;
}

void Fader::Stop()
{
  IsFading = false;
}
