/*
  ==============================================================================

    Voice.cpp
    Created: 22 Sep 2020 5:21:13pm
    Author:  jacop

  ==============================================================================
*/

#include "Voice.h"
#include <iostream>

Voice::Voice(ADSR::Parameters* params)
{
    this->envelope.setParameters(*params);
    this->currentSampleIdx = 0;
    this->cloud = nullptr;
    this->grainCounter = 0;
    this->totalHops = 0;
}

Voice::~Voice()
{
}

// Must return true if this voice object is capable of playing the given sound. Always true in this case cause we only have one type of sound
bool Voice::canPlaySound(SynthesiserSound*)
{
    return true;
}

// Called to start a new note.This will be called during the rendering callback, so must be fast and thread-safe.
void Voice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{
    this->currentSampleIdx = 0;
    this->cloud = dynamic_cast<GrainCloud*>(sound);
    activeGrains.add(cloud->getNextGrain(nullptr));
    this->envelope.noteOn(); // Starts the attack phase of the envelope
    
}

// Called to stop a note. This will be called during the rendering callback, so must be fastand thread-safe.
void Voice::stopNote(float velocity, bool allowTailOff)
{

    /*if (allowTailOff) {
        this->envelope.noteOff();      //Starts the release phase of the envelope
    }
    else*/
        clearCurrentNote();
}

// Called to let the voice know that the pitch wheel has been moved. This will be called during the rendering callback, so must be fastand thread-safe.
void Voice::pitchWheelMoved(int newPitchWheelValue)
{
    // do something
}

// Called to let the voice know that a midi controller has been moved. This will be called during the rendering callback, so must be fastand thread-safe.
void Voice::controllerMoved(int controllerNumber, int newControllerValue)
{
    // do something
}

// Renders the next block of data for this voice. 
// The output audio data must be added to the current contents of the buffer provided. Only the region of the buffer between startSample and (startSample + numSamples) should be altered by this method.
    // If the voice is currently silent, it should just return without doing anything.
    // If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished.
    // The size of the blocks that are rendered can change each time it is called, 
    //and may involve rendering as little as 1 sample at a time.In between rendering callbacks, the voice's methods will be called to tell it about note and controller events. 
void Voice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    float sampleValue = 0;
    // If the voice is playing
    if (this->isVoiceActive()) {
        // Cycle trough all the samples of the buffer
        for (int samplePos = startSample; samplePos < startSample + numSamples; samplePos++) { 
            //DBG("1stGRAIN->LENGTH: " + std::to_string(activeGrains.getFirst()->getLength()));
            //DBG("totalhops: " + std::to_string(totalHops));
            //ADD GRAINS // do we need to add a new grain on active grains?
            if (currentSampleIdx == this->cloud->nextInterOnset(activeGrains.getLast()) + this->totalHops) {
                activeGrains.add(cloud->getNextGrain(activeGrains.getLast()));
                this->totalHops += this->cloud->nextInterOnset(activeGrains.getFirst());
               // DBG("<--------------ADDED GRAIN------------>");
                this->grainCounter++;
            }
            //REMOVE GRAINS if we get to the end of the grain then go to the next
            if (currentSampleIdx == activeGrains.getFirst()->getLength()) { 

                int hopsize = this->cloud->nextInterOnset(activeGrains.getFirst());
                //removes the first grain
                activeGrains.remove(0);
                //sets the currentSample Index to the starting position
                //that is going back #hopsize samples!
                currentSampleIdx -=hopsize; 

                //remove nextonset of the first
                this->totalHops -= hopsize;
                //DBG("<------------REMOVED GRAIN-------------->" );
            }
            
            if (this->envelope.isActive()) {                                                                            
                // If the envelope has not finished
                auto currentEnvelope = this->envelope.getNextSample();
                int hopSizeSum = 0;
                // For each channel of the output buffer
                //DBG("active grains size:" + std::to_string(this->activeGrains.size()));
                for (auto i = 0; i < outputBuffer.getNumChannels(); i++) {    
                    sampleValue = 0;
                    hopSizeSum = 0.0;                                           
                    //compute the sum of the samples from all the currently active grains
                    for(auto grain : activeGrains){
                        //DBG("COUNTER:" + std::to_string(this->grainCounter));
                        if (currentSampleIdx < grain->getLength()) {
                            //DBG("getSample IDX :" + std::to_string(currentSampleIdx - hopSizeSum));
                        //    DBG("hopSizeSum : " + std::to_string(hopSizeSum));
                            sampleValue += grain->getSample(i % grain->getNumChannels(), currentSampleIdx - hopSizeSum);
                            //DBG("sample value on channel " + std::to_string(i) + ": " + std::to_string(sampleValue));
                            //DBG("bufferMagnitude: " + std::to_string(grain->maxValue));
                            hopSizeSum += this->cloud->nextInterOnset(grain);
                        }
                        else std::cout << "error: out of grain!\n";
                    }

                    // aplly Global Envelope to the summed samples
                    //DBG("total sum of samples:" + std::to_string(sampleValue));
                    sampleValue *= currentEnvelope;     
                   //DBG("total sum of samples: (After envelope)" + std::to_string(sampleValue) + "\n");
                    // Write the sample. It mixes the currentSampleIdx with the one already present (written by other voices)  
                    outputBuffer.addSample(i, samplePos, sampleValue);                                                
                }
                this->currentSampleIdx++;
            }
            else {                                                                                                      // If the envelope has finished
                this->clearCurrentNote();                                                                               // Clear the note
                this->activeGrains.clear();                                                                         // clear all the activeGrains when note has been released

            }
        }
    }
    else {              // If the voice is not playing
        this->currentSampleIdx = 0;
        return;         // Return
    }
}

// CUSTOM FUNCTIONS

void Voice::setEnvelope(ADSR::Parameters* params)
{
    this->envelope.setParameters(*params);
}

void Voice::setEnvelopeSampleRate(double sampleRate)
{
    this->envelope.setSampleRate(sampleRate);
}

float Voice::clip(float value, float min, float max)
{
    return std::max(min, std::min(value, max));
}
