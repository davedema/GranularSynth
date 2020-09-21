/*
  ==============================================================================

    Scheduler.h
    Created: 17 Aug 2020 11:50:43am
    Author:  daveg

  ==============================================================================
*/

/*
    Maintains state necessary for activating grains according to Grain onset times and durationssupplied by a SequenceStrategy.
    Exposes a method for synthesizing samples of sound by mixing together the output from its activeGrains.
    Manages grain allocation, for efficiency it may maintain a pool of reusable Grains.
*/

#include "SequenceStrategy.h"
#include "Grain.h"
#include "FileLoader.h"
#pragma once


class Scheduler {

private:
 
    SequenceStrategy strategy;
    std::vector<Grain*> grains;

    
public:

    void synthesize();

};