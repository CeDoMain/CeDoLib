#include "AdafruitTftTabControl.h"

TftTabControl::TftTabControl(word left, word top, word width, word height, byte tabCount, word ribbonWidth)
    : TftElement(left, top, width, height), SelectedTab(0), TabCount(tabCount),
      RibbonWidth(ribbonWidth), ButtonHeight((Height - BorderThickness) / TabCount)
{
    Pages = new TftContainer*[TabCount];
    ContentDrawer = new Delegate<void, Adafruit_GFX*, word, word, bool>*[TabCount];
    for (int i = 0; i < TabCount; i++)
    {
        Pages[i] = new TftContainer(0, 0, Width - RibbonWidth, Height);
        ContentDrawer[i] = 0;
    }
}
void TftTabControl::Begin()
{
    for (int i = 0; i < TabCount; i++)
    {
        Pages[i]->Begin();
    }
}
void TftTabControl::Update()
{
    Pages[SelectedTab]->Update();
}
void TftTabControl::SetParent(Adafruit_GFX* gfx, word offsetX, word offsetY)
{
    TftElement::SetParent(gfx, offsetX, offsetY);
    for (int i = 0; i < TabCount; i++)
    {
        Pages[i]->SetParent(Gfx, Left, Top);
    }
}
void TftTabControl::Touch(word x, word y)
{
    if (Right - RibbonWidth < x)
    {
        // Button wurde berührt
        word yTest = Top + BorderThickness / 2 + ButtonHeight;
        for (int i = 0; i < TabCount; i++)
        {
            if (y <= yTest)
            {
                if (i != SelectedTab)
                    SelectTab(i);
                return;
            }
            yTest += ButtonHeight;
        }
    }
    else
    {
        // Seite wurde berührt
        Pages[SelectedTab]->Touch(x, y);
    }
}
void TftTabControl::SetVisibility(bool value)
{
    Pages[SelectedTab]->SetVisibility(value);
    TftElement::SetVisibility(value);
}
void TftTabControl::SelectTab(byte index)
{
    if (SelectedTab == index)
        return;
    Pages[SelectedTab]->SetVisibility(false);
    SelectedTab = index;
    if (GetVisibility())
    {
        Draw();
        Pages[SelectedTab]->SetVisibility(true);
    }
}
void TftTabControl::Add(byte index, TftElement* e)
{
    Pages[index]->Add(e);
}
void TftTabControl::SetButtonContent(byte index, Delegate<void, Adafruit_GFX*, word, word, bool>* contentDrawer)
{
    ContentDrawer[index] = contentDrawer;
}
void TftTabControl::Draw()
{
    word buttonLeft = Right + 1 - RibbonWidth + BorderThickness;

    // oberste horizontale Linie zeichnen
    Gfx->fillRect(buttonLeft, Top, RibbonWidth - BorderThickness, BorderThickness, SelectedTab == 0 ? TouchForeground : ControlForeground);

    for (int i = 0; i < TabCount; i++)
    {
        bool isSelected = i == SelectedTab;

        // Ausfüllen
        Gfx->fillRect(buttonLeft, Top + BorderThickness + i * ButtonHeight, RibbonWidth - BorderThickness, ButtonHeight - BorderThickness, isSelected ? ContainerBackground : ControlBackground);

        // untere horizontale Linie zeichnen
        Gfx->fillRect(buttonLeft, Top + ButtonHeight + i * ButtonHeight, RibbonWidth - BorderThickness, BorderThickness, (isSelected || SelectedTab == i + 1) ? TouchForeground : ControlForeground);

        // Inhalt zeichnen
        if (ContentDrawer[i] != 0)
            (*ContentDrawer[i])(Gfx, buttonLeft + (RibbonWidth - BorderThickness) / 2, Top + BorderThickness + i * ButtonHeight + (ButtonHeight - BorderThickness) / 2, isSelected);
    }

    // Rand der Buttonspalte zeichnen
    Gfx->fillRect(Right - RibbonWidth + 1, Top, BorderThickness, BorderThickness + SelectedTab * ButtonHeight, TouchForeground);
    Gfx->fillRect(Right - RibbonWidth + 1, Top + BorderThickness + SelectedTab * ButtonHeight, BorderThickness, ButtonHeight - BorderThickness, ContainerBackground);
    Gfx->fillRect(Right - RibbonWidth + 1, Top + (SelectedTab + 1) * ButtonHeight, BorderThickness, BorderThickness + (TabCount - SelectedTab) * ButtonHeight, TouchForeground);
}