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
    return int(currentGrain->getLength()/2);
}

int SequenceStrategy::nextInterOnset(Grain* currentGrain, int currentAverageTime, Grain* nextGrain, int nextAverageTime)
{
    return 0;
}

int SequenceStrategy::nextInterOnset(Grain* currentGrain, Grain* nextGrain, float userLength)
{
    return 0;
}

Array<float> SequenceStrategy::computeCrossCorrelation(Grain* currentGrain, int currentAverageTime, Grain* nextGrain, int nextAverageTime)
{
    return Array<float>();
}

Array<float> SequenceStrategy::computeCrossCorrelation(Grain* currentGrain, Grain* nextGrain, float userLength)
{
    return Array<float>();
}