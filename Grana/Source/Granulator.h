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
#include "Model.h"
#include "GrainCloud.h"

class Granulator
{
public:
    Granulator();
    ~Granulator();
    //GrainCloud* getCloud();
    void initialize(int fileLength);
    void process(AudioBuffer<float>& outputBuffer, int numSamples);
    void setSampleRate(double sampleRate);

    //void setSamplesPerBlock(int samplesPerBlock);
    void setModel(Model* model);

private:
    //GrainCloud cloud;               // Grain pool derived from the loaded sample
    Array<Grain*> activeGrains;     // Grains to be played (extracted from the cloud)
    SequenceStrategy strategy;
    Model* model;
    int nextOnset;      //Tells us when the next grain should play
    int position;       //Position in the audio file
    int fileLength;    //Total length of the audio file

    //int currentSampleIdx;           // Position in samples starting from the first grain in activeGrains (for managing grains splitted in multiple output buffers)
    //int totalHops;                  // Sum of the hop sizes of the active grains


    /*=============================================================================
    The following two pointers are initialized to the first Grain and then proceed 
    one next to the other to get interonsets 
    ===============================================================================*/
    /*
    Grain* lastActivatedGrain;      //points to the last grain activated 
    Grain* nextActivatedGrain;      //points to the next grain to activate

    Array<int> interOnsets;
    Array<AudioBuffer<float>*> freqShiftedGrains;  //dinamically created frequency shifted buffers


    dsp::LinkwitzRileyFilter<float> masterLowPassFilter;  //the combination is a bandpass
    dsp::LinkwitzRileyFilter<float> masterHighPassFilter;

    int samplesPerBlock;
    */
};