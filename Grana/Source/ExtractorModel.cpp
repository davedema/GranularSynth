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
    setHasSentUpdate(true);
    this->lastGrainsPushed = nullptr;
    this->lastOutputBufferPushed = nullptr;
}

ExtractorModel::~ExtractorModel()
{
}

Array<AudioBuffer<float>*> ExtractorModel::getLastGrainsPushed()
{
    MessageManagerLock mml(Thread::getCurrentThread()); //this ensures that only one thread at a time computes spectrum
    if (!mml.lockWasGained())
    {
        return nullptr;
    }
    return this->lastGrainsPushed;
}

Array<float>* ExtractorModel::getLastOutputBufferPushed()
{
    MessageManagerLock mml(Thread::getCurrentThread());
    if (!mml.lockWasGained())
    {
        return nullptr;
    }
    return this->lastOutputBufferPushed;
}

bool ExtractorModel::pushBuffers(Array<Grain*>* grains, Array<float> outputBuffer, ExtractorModel* extractorModel)
{
    MessageManagerLock mml(Thread::getCurrentThread()); 
    if (!mml.lockWasGained())
    {
        return false;
    }
    if (!extractorModel->getHasSentUpdate())
        return false;

    clearMemory(extractorModel);
    extractorModel->lastGrainsPushed.clearQuick();
    for (auto grain : *grains)
        grain != nullptr ? extractorModel->lastGrainsPushed.add(new AudioBuffer<float>(*grain->getBuffer())) : 0;

    extractorModel->lastOutputBufferPushed = new Array<float>(outputBuffer);
    extractorModel->setHasSentUpdate(false);

    return true;
}

bool ExtractorModel::clearMemory(ExtractorModel* extractorModel)
{
    for (auto buff : extractorModel->lastGrainsPushed)
        delete buff;
    delete extractorModel->lastOutputBufferPushed;
    return true;
}

bool ExtractorModel::setHasSentUpdate(bool hasSentUpdate)
{
    this->hasSentUpdate = hasSentUpdate;
    return true;
}

bool ExtractorModel::getHasSentUpdate()
{
    MessageManagerLock mml(Thread::getCurrentThread());
    if (!mml.lockWasGained())
    {
        return false;
    }
    return this->hasSentUpdate;
}
