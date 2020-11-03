/*
  ==============================================================================

    ExtractorModel.cpp
    Created: 2 Nov 2020 11:17:08pm
    Author:  jacop

    Model for the extractor class. Data of this class will be used in separate
    threads in the Extractor class
  ==============================================================================
*/

#include "ExtractorModel.h"

ExtractorModel::ExtractorModel()
{
    this->hasSentUpdate = true;
}

ExtractorModel::~ExtractorModel()
{
}

Array<AudioBuffer<float>>* ExtractorModel::getLastGrainsPushed()
{
    return &this->lastGrainsPushed;
}

AudioBuffer<float>* ExtractorModel::getLastOutputBufferPushed()
{
    return &this->lastOutputBufferPushed;
}

void ExtractorModel::pushBuffers(Array<Grain*>* grains)
{
    if (!hasSentUpdate)
        return;

    lastGrainsPushed.clearQuick();
    for (auto grain : *grains)
        this->lastGrainsPushed.add(AudioBuffer<float>(*grain->getBuffer()));
}

void ExtractorModel::setHasSentUpdate(bool hasSentUpdate)
{
    this->hasSentUpdate = hasSentUpdate;
}

bool ExtractorModel::getHasSentUpdate()
{
    return this->hasSentUpdate;
}
