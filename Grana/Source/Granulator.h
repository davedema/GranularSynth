/*
  ==============================================================================

    Granulator.h
    Created: 20 Sep 2020 11:16:43am
    Author:  gabri

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SequenceStrategy.h"
#include "GrainCloud.h"

class Granulator
{
private:
    GrainCloud cloud;               // Grain pool derived from the loaded sample
    Array<Grain*> activeGrains;     // Grains to be played (extracted from the cloud)
    SequenceStrategy strategy;
    int currentSampleIdx;           // Position in samples starting from the first grain in activeGrains (for managing grains splitted in multiple output buffers)
    int totalHops;                  // Sum of the hop sizes of the active grains


    /*=============================================================================
    The following two pointers are initialized to the first Grain and then proceed 
    one next to the other to get interonsets 
    ===============================================================================*/
    Grain* lastActivatedGrain;      //points to the last grain activated 
    Grain* nextActivatedGrain;      //points to the next grain to activate

    Array<int> interOnsets;

public:
    Granulator();
    ~Granulator();
    GrainCloud* getCloud();
    void initialize();
    void process(AudioBuffer<float>& outputBuffer, int numSamples);
};