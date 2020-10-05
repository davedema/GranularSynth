/*
  ==============================================================================

    SequenceStrategy.cpp
    Created: 5 Oct 2020 6:26:16pm
    Author:  jacop

  ==============================================================================
*/

#include "SequenceStrategy.h"

SequenceStrategy::SequenceStrategy() 
{
    //TODO: getrawvalues from treestate
    this->quasiSyncRange = 0;
}

int SequenceStrategy::nextInterOnset()
{
    float randomNumber = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) * 2.0f; //rand in [-1, 1]
    return (1 / grainDensity) + randomNumber * quasiSyncRange;
}
