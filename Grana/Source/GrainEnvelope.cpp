/*
  ==============================================================================

    GrainEnvelope.cpp
    Created: 24 Sep 2020 5:22:50pm
    Author:  Davide De Marco

  ==============================================================================
*/

#include "GrainEnvelope.h"

float GrainEnvelope::currentValue(float time)
{
    return kernel[(int)time * sampleRate];
}

std::vector<double> GrainEnvelope::getKernel()
{
    return this->kernel;
}
