/*
  ==============================================================================

    Granulator.h
    Created: 20 Sep 2020 11:16:43am
    Author:  gabri

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "MasterEnvelope.h"
#include "Voice.h"

#define VOICES 10       // Number of polyphony voices

class Granulator : public juce::Synthesiser
{
private:
    ADSR::Parameters envelopeParams;       // Envelope parameters, needed for setting

public:
    Granulator();
    ~Granulator();
    void setEnvelopeSampleRate(double sampleRate);          // Set the envelope sample rate
    void setEnvelope(float a, float d, float s, float r);   // Set the envelope parameters and apply them to the actual envelope

};