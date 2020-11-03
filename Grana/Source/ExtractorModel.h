/*
  ==============================================================================

    ExtractorModel.h
    Created: 2 Nov 2020 11:17:08pm
    Author:  jacop

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Grain.h"

class ExtractorModel {

public:
    ExtractorModel();
    ~ExtractorModel();

    Array<AudioBuffer<float>>* getLastGrainsPushed();
    AudioBuffer<float>* getLastOutputBufferPushed();

    void pushBuffers(Array<Grain*>* grains);

    void setHasSentUpdate(bool hasSentUpdate);
    bool getHasSentUpdate();

private:
    Array<AudioBuffer<float>> lastGrainsPushed;
    AudioBuffer<float> lastOutputBufferPushed;

    bool hasSentUpdate;
};