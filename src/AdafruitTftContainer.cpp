#include "AdafruitTftContainer.h"

TftContainer::TftContainer(word left, word top, word width, word height)
    : TftElement(left, top, width, height)
{
    
}
void TftContainer::Begin()
{
    Elements.ForEach([](TftElement* e)
    {
        e->Begin();
    });
}
void TftContainer::Update()
{
    Elements.ForEach([](TftElement* e)
    {
        e->Update();
    });
}
void TftContainer::Touch(word x, word y)
{
    TftElement* e = Elements.Find([=](TftElement* e) -> bool
    {
        return e->GetVisibility() && e->Left <= x && x <= e->Right && e->Top <= y && y <= e->Bottom;
    });
    if (e != 0)
        e->Touch(x, y);
}
void TftContainer::SetVisibility(bool value)
{
    TftElement::SetVisibility(value);
    Elements.ForEach([=](TftElement* e)
    {
        e->SetVisibility(value);
    });
}
void TftContainer::Add(TftElement* e)
{
    e->SetParent(Gfx, Left, Top);
    Elements.Add(e);
}
void TftContainer::Draw()
{
    Gfx->fillRect(Left, Top, Width, Height, ContainerBackground);
}