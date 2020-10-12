/*
  ==============================================================================

    Voice.cpp
    Created: 22 Sep 2020 5:21:13pm
    Author:  jacop

  ==============================================================================
*/

#include "Voice.h"

Voice::Voice(ADSR::Parameters* params)
{
    this->envelope.setParameters(*params);
    this->currentSampleIdx = 0;
    this->cloud = nullptr;
    this->currentGrain = nullptr;
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
    if (allowTailOff)
        this->envelope.noteOff();      //Starts the release phase of the envelope
    else
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
    auto sampleValue;
    // If the voice is playing
    if (this->isVoiceActive()) {
        // Cycle trough all the samples of the buffer
        for (int samplePos = startSample; samplePos < startSample + numSamples; samplePos++) {                          
            //if we get to the end of the grain then go to the next
            if (currentSampleIdx == currentGrain->getLength()) { 

                int hopsize = activeGrains.getFirst().getNextOnSetTime();
                //removes the first grain
                activeGrains.remove(0);
                //sets the currentSample Index to the starting position
                //that is going back #hopsize samples!
                currentSampleIdx -=hopsize; 
            }



            //do we need to add a new grain on active grains?
            if(currentSampleIdx >= activeGrains.getLast()->getNextOnSetTime()){
                activeGrains.add(cloud->getNextGrain(activeGrains.getLast()))
            }
            
            if (this->envelope.isActive()) {                                                                            
                // If the envelope has not finished
                auto currentEnvelope = this->envelope.getNextSample();
                int hopSizeSum = 0;
                // For each channel of the output buffer
                for (auto i = 0; i < outputBuffer.getNumChannels(); i++) {    
                    hopSizeSum = 0;                                           
                    //compute the sum of the samples from all the currently active grains
                    while(auto grain : activeGrains){
                        sampleValue += grain.getSample(i, currentSampleIdx - hopSizeSum)
                        hopSizeSum += grain.getNextOnSetTime();
                    }

                    // aplly Global Envelope to the summed samples
                    sampleValue *= currentEnvelope;         
                    // Write the sample. It mixes the currentSampleIdx with the one already present (written by other voices)  
                    outputBuffer.addSample(i, samplePos, sampleValue);                                                
                }
                this->currentSampleIdx++;
            }
            else {                                                                                                      // If the envelope has finished
                this->clearCurrentNote();                                                                               // Clear the note
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
