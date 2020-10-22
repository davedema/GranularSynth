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

int SequenceStrategy::nextInterOnset(int userLength)
{
    float spreadControl = this->quasiSyncRange * distribution(engine);
    return userLength + (int)spreadControl;
}

int SequenceStrategy::nextInterOnset(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength)
{
    if (currentBuffer == nextBuffer) //handle single buffer case
        return nextInterOnset(userLength);

    Array<float>* correlationArray = computeCrossCorrelation(currentBuffer, nextBuffer, userLength, grainLength);
    int interOnset = (int)std::distance(correlationArray->begin(), std::max_element(correlationArray->begin(), correlationArray->end()))
        + userLength;
    delete correlationArray;
    float spreadControl = this->quasiSyncRange * distribution(engine);
    return interOnset + (int)spreadControl;
}

Array<float>* SequenceStrategy::computeCrossCorrelation(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength)
{
    Array<float>* correlationArray = new Array<float>();
    int numChannels = currentBuffer->getNumChannels();
    correlationArray->clear();

    //begin compute autocorrelation
    for (int i = 0; i < userLength; i++) {
        float totalValue = 0;
        for (int channel = 0; channel > numChannels; channel++) {
            float newValue = 0;
            for (int j = 0; j < grainLength - i - 1; j++) {
                newValue += currentBuffer->getSample(channel, userLength + j) * nextBuffer->getSample(channel, j) / (grainLength - userLength);
            }
            totalValue += newValue;
        }
        totalValue /= numChannels; //average over channels
        correlationArray->add(totalValue);
    }
    //end compute autocorrelation

    return correlationArray;
}