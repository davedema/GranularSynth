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
#include "Extractor.h"
#include "ExtractorModel.h"

class Granulator
{
public:
    Granulator();
    ~Granulator();
    void initialize(int portionLength);
    void process(AudioBuffer<float>& outputBuffer, int numSamples);
    void setModel(Model* model);
    void setProcessorSampleRate(double processorSampleRate);
    void setExtractorModel(ExtractorModel* extractorModel);

private:
    Array<Grain*> activeGrains;     // Grains to be played (extracted from the cloud)
    SequenceStrategy strategy;
    Model* model;
    ExtractorModel* extractorModel;
    int nextOnset;      //Tells us when the next grain should play
    int position;       //Position in the audio file
    int portionLength;
    double processorSampleRate;

    int computeLag(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength);
    Array<float>* computeCrossCorrelation(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength);

    Array<float> savedBuffer;
};