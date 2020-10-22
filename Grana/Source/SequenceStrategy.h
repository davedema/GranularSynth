/*
  ==============================================================================

    SequenceStrategy.h
    Created: 15 Sep 2020 2:20:49pm
    Author:  jacop

  ==============================================================================

  Provides an interface allowing the Scheduler to determine when the next grain should occur andwhat its duration should be

*/

#pragma once

#include <stdlib.h>
#include <random>
#include "Grain.h"

class SequenceStrategy {


private:
    float grainDensity;
    int grainLength;
    float quasiSyncRange;

    Array<float> computeCrossCorrelation(Grain* currentGrain, int currentAverageTime, Grain* nextGrain, int nextAverageTime);
    Array<float> computeCrossCorrelation(Grain* currentGrain, Grain* nextGrain, float userLength);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 engine; //Standard mersenne_twister_engine seeded with rd() and feeded to distribution
    std::uniform_real_distribution<float> distribution; //uniform distribution

public:

    SequenceStrategy();
    int nextInterOnset(Grain* currentGrain);
    int nextInterOnset(Grain* currentGrain, int currentAverageTime, Grain* nextGrain, int nextAverageTime);
    int nextInterOnset(Grain* currentGrain, Grain* nextGrain, float userLength);

};