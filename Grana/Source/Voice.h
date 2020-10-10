/*
  ==============================================================================

    Voice.h
    Created: 22 Sep 2020 5:21:13pm
    Author:  jacop

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GrainCloud.h"

class Voice : public juce::SynthesiserVoice
{
private:

    ADSR envelope;         // A pointer to the global envelope (needed for rendering)
    GrainCloud* cloud;
    int currentSample;

public:
    Voice(ADSR::Parameters* params);
    ~Voice();

    // Pure virtual functions
    bool canPlaySound(SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;

    // Custom functions
    void setEnvelope(ADSR::Parameters* params);
    void setEnvelopeSampleRate(double sampleRate);

};