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
    lastActivatedGrain = activeGrains.getFirst();
    nextActivatedGrain = activeGrains.getFirst();
    interOnsets.add(this->strategy.nextInterOnset( //add first interonset
        lastActivatedGrain->getFreqShiftedGrains()[38],
        nextActivatedGrain->getFreqShiftedGrains()[38],
        lastActivatedGrain->getLength() / 2,
        lastActivatedGrain->getLength()
    ));
}

// Process the sound
void Granulator::process(AudioBuffer<float>& outputBuffer, int numSamples)
{
    if (activeGrains.isEmpty())
        return;
    float sampleValue = 0;              // Output sample value

    // Cycle trough all the samples of the buffer
    for (int samplePos = 0; samplePos < numSamples; samplePos++) {
        int grainLength = lastActivatedGrain->getBuffer()->getNumSamples();
        int firstInterOnset = interOnsets[0];
        int lastInterOnset = interOnsets.getLast();
        // ADD GRAINS: If the current sample is the Hop size of the last active grain, get the next grain to play
        if (currentSampleIdx == lastInterOnset + this->totalHops) {
            if (lastActivatedGrain == nextActivatedGrain) { //if first grain
                interOnsets.remove(0); //remove first onset
                interOnsets.add(1);    //TODO: verify
            }
            lastActivatedGrain = activeGrains.getLast();
            activeGrains.add(this->cloud.getNextGrain(activeGrains.getLast()));
            nextActivatedGrain = activeGrains.getLast();
            this->totalHops += lastInterOnset;
            interOnsets.add(this->strategy.nextInterOnset( //add interonset
                lastActivatedGrain->getFreqShiftedGrains()[38],
                nextActivatedGrain->getFreqShiftedGrains()[38],
                lastActivatedGrain->getLength() / 2,
                lastActivatedGrain->getLength()
            ));
        }

        //REMOVE GRAINS: if we get to the end of the grain then delete it and go to the next
        if (currentSampleIdx == activeGrains.getFirst()->getLength()) {
            activeGrains.remove(0);
            // Updating indexes
            currentSampleIdx -= firstInterOnset;
            this->totalHops -= firstInterOnset;
            interOnsets.remove(0);
        }

        int hopSizeSum = 0;
        // For each channel of the output buffer
        for (auto i = 0; i < outputBuffer.getNumChannels(); i++) {
            sampleValue = 0;
            hopSizeSum = 0.0;

            Grain* previous = nullptr;
            // Compute the sum of the samples from all the currently active grains
            int k = 0;
            for (auto grain : activeGrains) {
                sampleValue += grain->getSample(i % grain->getNumChannels(), currentSampleIdx - hopSizeSum);
                hopSizeSum += interOnsets[k];
                ++k;
            }
            outputBuffer.setSample(i, samplePos, sampleValue);
        }
        this->currentSampleIdx++;
    }
}

void Granulator::setSamplesPerBlock(int samplesPerBlock)
{
    this->samplesPerBlock = samplesPerBlock;
}


