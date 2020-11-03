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

    Array<AudioBuffer<float>*> getLastGrainsPushed();
    Array<float>* getLastOutputBufferPushed();

    static bool pushBuffers(Array<Grain*>* grains, Array<float> outputBuffer, ExtractorModel *extractorModel);
    static bool clearMemory(ExtractorModel* extractorModel);

    bool setHasSentUpdate(bool hasSentUpdate);
    bool getHasSentUpdate();

    

protected:
    Array<AudioBuffer<float>*> lastGrainsPushed;
    Array<float>* lastOutputBufferPushed;

private:
    bool hasSentUpdate;

    
};