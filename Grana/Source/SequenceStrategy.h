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



class SequenceStrategy {


private:
    float grainDensity;
    int grainLength;
    float quasiSyncRange;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 engine; //Standard mersenne_twister_engine seeded with rd() and feeded to distribution
    std::uniform_real_distribution<float> distribution; //uniform distribution

    

public:

    SequenceStrategy();
    int nextDuration(); //TODO: check if int or float (depending if samples or time)
    int nextInterOnset();

};