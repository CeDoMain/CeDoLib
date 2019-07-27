#include "AdafruitTftElement.h"

TftElement::TftElement(word left, word top, word width, word height)
    : Left(left), Right(left + width - 1), Top(top), Bottom(top + height - 1), Width(width), Height(height), Gfx(0)
{
    IsVisible = false;
}
void TftElement::Begin()
{
    
}
void TftElement::Update()
{
    
}
void TftElement::SetParent(Adafruit_GFX* gfx, word offsetX, word offsetY)
{
    Gfx = gfx;
    Left += offsetX;
    Right += offsetX;
    Top += offsetY;
    Bottom += offsetY;
}
void TftElement::SetVisibility(bool value)
{
    IsVisible = value;
    if (value)
        Draw();
}
bool TftElement::GetVisibility()
{
    return IsVisible;
}
void TftElement::DrawTriangle(word l, word t, word w, word h, byte r, Color565 f, Color565 b)
{
    word x0, y0, x1, y1, x2, y2;
    x0 = x1 = x2 = l;
    y0 = y1 = y2 = t;
    switch (r)
    {
        case 1:
            // Spitze rechts
            x1 += w;
            y1 += h / 2;
            y2 += h;
            break;
        
        case 2:
            // Spitze unten
            x1 += w;
            x2 += w / 2;
            y2 += h;
            break;
        
        case 3:
            // Spitze links
            x2 = x0 += w;
            y1 += h / 2;
            y2 += h;
            break;

        default:
            // Spitze oben
            x0 += w / 2;
            x2 += w;
            y1 = y2 += h;
            break;
    }
    Gfx->fillTriangle(x0, y0, x1, y1, x2, y2, f);
    if (f != b)
    {
        // Inneres Dreieck muss wegen den schrägen Kanten weiter als die Randbreite in die Mitte verschoben werden
        byte offsetX = (float)BorderThickness * ((float)w / h / 2) * (2 - 1 / ((float)w / h / 2 + 1));
        byte offsetY = (float)BorderThickness * ((float)h / w / 2) * (2 - 1 / ((float)h / w / 2 + 1));
        switch (r)
        {
            case 1:
                // Spitze rechts
                x0 = x2 += BorderThickness;
                x1 -= BorderThickness + 2 * offsetX;
                y0 += BorderThickness + offsetY;
                y2 -= BorderThickness + offsetY;
                break;
            
            case 2:
                // Spitze unten
                x0 += BorderThickness + offsetX;
                x1 -= BorderThickness + offsetX;
                y0 = y1 += BorderThickness;
                y2 -= BorderThickness + 2 * offsetY;
                break;
            
            case 3:
                // Spitze links
                x2 = x0 -= BorderThickness;
                x1 += BorderThickness + 2 * offsetX;
                y0 += BorderThickness + offsetY;
                y2 -= BorderThickness + offsetY;
                break;

            default:
                // Spitze oben
                x1 += BorderThickness + offsetX;
                x2 -= BorderThickness + offsetX;
                y0 += BorderThickness + 2 * offsetY;
                y1 = y2 -= BorderThickness;
                break;
        }
        Gfx->fillTriangle(x0, y0, x1, y1, x2, y2, b);
    }
}
void TftElement::DrawRect(word l, word t, word w, word h, Color565 f, Color565 b)
{
    if (f == b)
    {
        Gfx->fillRect(l, t, w, h, f);
    }
    else
    {
        // Rand zeichnen (mit 1px Überlappung nach innen)
        for (int i = 0; i < BorderThickness; i++)
        {
            Gfx->drawRect(l + i, t + i, w - 2 * i, h - 2 * i, f);
        }

        // Füllung zeichnen
        Gfx->fillRect(l + BorderThickness, t + BorderThickness, w - 2 * BorderThickness, h - 2 * BorderThickness, b);
    }
}