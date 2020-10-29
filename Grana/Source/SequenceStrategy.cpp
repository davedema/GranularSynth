/*
  ==============================================================================

    SequenceStrategy.cpp
    Created: 5 Oct 2020 6:26:16pm
    Author:  jacop

  ==============================================================================
*/

#include "SequenceStrategy.h"

SequenceStrategy::SequenceStrategy()
{
    //TODO: getrawvalues from treestate
    this->quasiSyncRange = 0;
    engine = std::mt19937(this->rd());
    distribution = std::uniform_real_distribution<float>(-1.0f, std::nextafter(1.0f, std::numeric_limits<float>::max())); //c++ docs
}

void SequenceStrategy::setModel(Model* model)
{
    this->model = model;
}

int SequenceStrategy::nextInterOnset(int userLength)
{
    float spreadControl = this->quasiSyncRange * distribution(engine);
    return userLength + (int)spreadControl;
}

int SequenceStrategy::nextInterOnset(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer)
{
    int grainLength = currentBuffer->getNumSamples();
    int userLength = model->getDensity() * grainLength / 100.0f;
    if (currentBuffer == nextBuffer) //handle single buffer case
        return nextInterOnset(userLength);

    Array<float>* correlationArray = computeCrossCorrelation(
        currentBuffer, 
        nextBuffer, 
        userLength, 
        grainLength
    ); //compute correlation
    float *maxvalue = std::max_element(
        correlationArray->begin(),
        correlationArray->end());
    float interOnset = (float)std::distance(correlationArray->begin(), std::max_element(
        correlationArray->begin(), 
        correlationArray->end())); //add lag
    interOnset += userLength;
    delete correlationArray;
    float spreadControl = this->quasiSyncRange * distribution(engine);
    interOnset += spreadControl; //add random
    if(interOnset < grainLength)
        crossFade(currentBuffer, nextBuffer, (int)interOnset, grainLength); //crossfade grains*/
    return interOnset;
}

void SequenceStrategy::setQuasiSyncRange(float quasiSyncRange)
{
    this->quasiSyncRange = quasiSyncRange;
}


Array<float>* SequenceStrategy::computeCrossCorrelation(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength)
{
    Array<float>* correlationArray = new Array<float>();
    int numChannels = currentBuffer->getNumChannels();
    correlationArray->clear();

    //begin compute autocorrelation
    for (int i = 0; i < grainLength - userLength; i++) {
        float totalValue = 0;
        for (int channel = 0; channel < numChannels; channel++) {
            float newValue = 0;
            for (int j = userLength; j < grainLength - i; j++) {
                newValue += currentBuffer->getSample(channel, j) * nextBuffer->getSample(channel, j - userLength);
            }
            newValue /= (float)(grainLength - userLength);
            totalValue += newValue;
        }
        totalValue /= numChannels; //average over channels
        correlationArray->add(totalValue);
    }
    //end compute autocorrelation
    return correlationArray;
}

void SequenceStrategy::crossFade(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int interOnset, int grainLength)
{
    currentBuffer->applyGainRamp(interOnset, grainLength - interOnset, 1, 0);
    nextBuffer->applyGainRamp(0, grainLength - interOnset, 0, 1);
}

void SequenceStrategy::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}