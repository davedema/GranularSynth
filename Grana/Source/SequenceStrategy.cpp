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
    engine = std::mt19937(this->rd());
    distribution = std::uniform_real_distribution<float>(-1.0f, std::nextafter(1.0f, std::numeric_limits<float>::max())); //c++ docs
}

int SequenceStrategy::nextInterOnset(Grain* currentGrain)
{
    /*
    float randomNumber = distribution(engine); //random number in [-1, 1]
    return (1 / grainDensity) + randomNumber * quasiSyncRange;
    */
    return currentGrain->getLength() / 2;
}
