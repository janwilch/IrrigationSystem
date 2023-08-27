#include "OnDelayTimer.hpp"
#include "Arduino.h"

OnDelayTimer::OnDelayTimer()
{
}

OnDelayTimer::~OnDelayTimer()
{
}

bool OnDelayTimer::IsElapsed(unsigned long ms)
{
    if (OnDelayTimer::currentDelta != ms)
    {
        OnDelayTimer::currentDelta = ms;
        OnDelayTimer::endMillis = ms + millis();
    }

    return millis() >= OnDelayTimer::endMillis;
}

void OnDelayTimer::Reset()
{
    OnDelayTimer::currentDelta = -1;
}
