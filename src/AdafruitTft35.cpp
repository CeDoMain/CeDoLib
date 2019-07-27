#include "AdafruitTft35.h"

AdafruitTft35::AdafruitTft35(signed char tftCsPin, signed char tftDcPin, unsigned long fadeOutDelay)
    : Tft(tftCsPin, tftDcPin), Touchscreen(Width, Height), KeysLocked(false),
    DisplayDimmer(fadeOutDelay), DisplayFader(), TopContainer(0, 0, Width, Height)
{     
    TopContainer.SetParent(&Tft, 0, 0);
}

void AdafruitTft35::Begin(AnalogOut* displayLightPin, DigitalOut* touchCsPin, unsigned long fadeOutTime, unsigned long fadeInTime)
{
    // Display Sperrautomatik
    DisplayFader.FadeEvent = new Delegate<void, decimal>([=](decimal intensity)
    {
        (*displayLightPin)(intensity / 10000.0);
    });
    DisplayFader.Start(fadeInTime, 0, 10000);
    DisplayDimmer.TimeIsUpEvent = new Delegate<>([=]()
    {
        KeysLocked = true;
        DisplayFader.Start(fadeOutTime, 0);
    });
    DisplayDimmer.Start();

    // Touchscreen initialisieren
    Touchscreen.Begin(touchCsPin);
    Touchscreen.SetRotation(1);
    Touchscreen.TouchEvent = new Delegate<void, unsigned short, unsigned short>([=](unsigned short x, unsigned short y)
    {
        DisplayDimmer.Start();

        if (KeysLocked)
        {
            DisplayFader.Start(fadeInTime, 10000);
            KeysLocked = false;
            return;
        }

        TopContainer.Touch(x, y);
    });

    // Bildschirm initialisieren
    Tft.begin(HX8357D);
    Tft.setRotation(1);
    Tft.fillScreen(HX8357_BLACK);

    // GUI initialisieren
    TopContainer.Begin();
    TopContainer.SetVisibility(true);
}

void AdafruitTft35::Update()
{
    Touchscreen.Update();
    DisplayDimmer.Update();
    DisplayFader.Update();
    TopContainer.Update();
}

void AdafruitTft35::Add(TftElement* e)
{
    TopContainer.Add(e);
}