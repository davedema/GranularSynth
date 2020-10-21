/*
  ==============================================================================

    Granulator.cpp
    Created: 20 Sep 2020 11:16:43am
    Author:  gabri

  ==============================================================================
*/
#include "Granulator.h"


Granulator::Granulator() 
{
    this->activeGrains.clearQuick();
    this->currentSampleIdx = 0;
    this->totalHops = 0;
}

Granulator::~Granulator()
{
    this->activeGrains.clearQuick();
}

GrainCloud* Granulator::getCloud()
{
    return &this->cloud;
}

// Initialize everything. Called when PLAY button is clicked
void Granulator::initialize()
{
    this->currentSampleIdx = 0;
    this->activeGrains.clearQuick();
    activeGrains.add(this->cloud.getNextGrain(nullptr));
}

// Process the sound
void Granulator::process(AudioBuffer<float>& outputBuffer, int numSamples)
{
    if (activeGrains.isEmpty())
        return;
    float sampleValue = 0;              // Output sample value

    // Cycle trough all the samples of the buffer
    for (int samplePos = 0; samplePos < numSamples; samplePos++) {

        // ADD GRAINS: If the current sample is the Hop size of the last active grain, get the next grain to play
        if (currentSampleIdx == this->strategy.nextInterOnset(activeGrains.getLast()) + this->totalHops) {
            activeGrains.add(this->cloud.getNextGrain(activeGrains.getLast()));
            this->totalHops += this->strategy.nextInterOnset(activeGrains.getFirst());
        }

        //REMOVE GRAINS: if we get to the end of the grain then delete it and go to the next
        if (currentSampleIdx == activeGrains.getFirst()->getLength()) {
            int hopsize = this->strategy.nextInterOnset(activeGrains.getFirst());
            activeGrains.remove(0);
            // Updating indexes
            currentSampleIdx -= hopsize;
            this->totalHops -= hopsize;
        }

        int hopSizeSum = 0;
        // For each channel of the output buffer
        for (auto i = 0; i < outputBuffer.getNumChannels(); i++) {
            sampleValue = 0;
            hopSizeSum = 0.0;

            // Compute the sum of the samples from all the currently active grains
            for (auto grain : activeGrains) {
                sampleValue += grain->getSample(i % grain->getNumChannels(), currentSampleIdx - hopSizeSum);
                hopSizeSum += this->strategy.nextInterOnset(grain);
            }
            outputBuffer.setSample(i, samplePos, sampleValue);
        }
        this->currentSampleIdx++;
    }
}


