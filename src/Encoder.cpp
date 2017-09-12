#include "Encoder.h"

Encoder::Encoder(const byte divider)
  : TrigA(0), TrigB(0), ValueA(false), ValueB(false), Counter(0), Divider(divider),
    CounterChangedEvent(), CountUpEvent(), CountDownEvent()
{
  
}
void Encoder::Begin(Trigger* triggerA, Trigger* triggerB)
{
  TrigA = triggerA;
  TrigB = triggerB;
  TrigA->Activated = Delegate<>(this, &Encoder::ActivateA);
  TrigA->Deactivated = Delegate<>(this, &Encoder::DeactivateA);
  TrigB->Activated = Delegate<>(this, &Encoder::ActivateB);
  TrigB->Deactivated = Delegate<>(this, &Encoder::DeactivateB);
}
void Encoder::Update()
{
  TrigA->Update();
  TrigB->Update();
}
void Encoder::Reset(long newValue)
{
  Counter = newValue;
  SubCount = 1;
  CounterChangedEvent(Counter);
}
long Encoder::GetCount()
{
  return Counter;
}
void Encoder::ActivateA()
{
  ValueA = true;
  ValueB ? CountUp() : CountDown();
}
void Encoder::DeactivateA()
{
  ValueA = false;
  ValueB ? CountDown() : CountUp();
}
void Encoder::ActivateB()
{
  ValueB = true;
  ValueA ? CountDown() : CountUp();
}
void Encoder::DeactivateB()
{
  ValueB = false;
  ValueA ? CountUp() : CountDown();
}
void Encoder::CountUp()
{
  SubCount++;
  if (SubCount > Divider)
  {
    SubCount = 1;
    Counter++;
    CounterChangedEvent(Counter);
  }
}
void Encoder::CountDown()
{
  SubCount--;
  if (SubCount == 0)
  {
    SubCount = Divider;
    Counter--;
    CounterChangedEvent(Counter);
  }
}
