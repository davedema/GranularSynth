/*
  ==============================================================================

    GrainEnvelope.cpp
    Created: 24 Sep 2020 5:22:50pm
    Author:  Davide De Marco

  ==============================================================================
*/

#include "GrainEnvelope.h"


float GrainEnvelope::currentValue(int time)
{
    return kernel[time];
}
