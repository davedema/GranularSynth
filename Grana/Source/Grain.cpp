/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"


WindowType Grain::getWinType() const
{
    return winType;
}

void Grain::setWinType(WindowType winType)
{
    this->winType = winType;
}

