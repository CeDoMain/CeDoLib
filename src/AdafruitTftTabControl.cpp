#include "AdafruitTftTabControl.h"

TftTabControl::TftTabControl(word left, word top, word width, word height, byte tabCount, word ribbonWidth)
    : TftElement(left, top, width, height), TabChangedEvent(0), SelectedTab(0), TabCount(tabCount),
      RibbonWidth(ribbonWidth), ButtonHeight((Height + BorderThickness) / TabCount)
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
        word yTest = Top + ButtonHeight / 2;
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
        if (TabChangedEvent != 0)
            (*TabChangedEvent)(SelectedTab);
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
    
    // Spalte mit Button Hintergrund und aktivierte Linie links
    Gfx->fillRect(buttonLeft, Top, RibbonWidth - BorderThickness, Height, ControlBackground);
    Gfx->fillRect(Right + 1 - RibbonWidth, Top, BorderThickness, Height, TouchForeground);

    // aktivierten Button wie Container färben und ohne linken Rand
    Gfx->fillRect(Right + 1 - RibbonWidth, Top + SelectedTab * ButtonHeight, RibbonWidth, ButtonHeight - BorderThickness, ContainerBackground);
    
    for (int i = 0; i < TabCount; i++)
    {
        bool isSelected = i == SelectedTab;

        // untere horizontale Linie zeichnen
        if (i != TabCount - 1)
            Gfx->fillRect(buttonLeft, Top + ButtonHeight - BorderThickness + i * ButtonHeight, RibbonWidth - BorderThickness, BorderThickness, (isSelected || SelectedTab == i + 1) ? TouchForeground : ControlForeground);

        // Inhalt zeichnen
        if (ContentDrawer[i] != 0)
            (*ContentDrawer[i])(Gfx, buttonLeft + (RibbonWidth - BorderThickness) / 2, Top + i * ButtonHeight + (ButtonHeight - BorderThickness) / 2, isSelected);
    }
}