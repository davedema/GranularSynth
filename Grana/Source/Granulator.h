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

class Granulator
{
public:
    Granulator();
    ~Granulator();
    void initialize(int portionLength);
    void process(AudioBuffer<float>& outputBuffer, int numSamples);
    void setSampleRate(double sampleRate);
    void setModel(Model* model);

private:
    Array<Grain*> activeGrains;     // Grains to be played (extracted from the cloud)
    SequenceStrategy strategy;
    Model* model;
    int nextOnset;      //Tells us when the next grain should play
    int position;       //Position in the audio file
    int portionLength;  //Total length of the portion to play
};