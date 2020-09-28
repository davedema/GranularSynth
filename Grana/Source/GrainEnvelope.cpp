/*
  ==============================================================================

    GrainEnvelope.cpp
    Created: 24 Sep 2020 5:22:50pm
    Author:  Davide De Marco

  ==============================================================================
*/

#include "GrainEnvelope.h"

GrainEnvelope* GrainEnvelope::instance = 0;


void GrainEnvelope::reset()
{
    if (getInstance() != nullptr) {
        delete getInstance(); // REM : it works even if the pointer is NULL (does nothing then)
        instance = nullptr; // so GetInstance will still work.
    }
}
