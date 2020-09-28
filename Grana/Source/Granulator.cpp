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
    this->envelopeParams.attack = 0.1;                  // Setting some default envelope parameters (should be taken from treestate)
    this->envelopeParams.decay = 0.1;
    this->envelopeParams.sustain = 0.7;
    this->envelopeParams.release = 0.1;

    for (int i; i < VOICES; ++i)                        // Add voices to the synth
        this->addVoice(new Voice(&this->envelopeParams));
    /*
    this->addSound(new GrainCloud());                   // Add the sound of the synth (that is the GrainCloud)
    */
}

Granulator::~Granulator()
{
    this->clearVoices();
    /*
    this->clearSounds();
    */
}

void Granulator::setEnvelopeSampleRate(double sampleRate)
{
    for (auto synthVoice : this->voices) {                  // Set the sample rate for all voices envelopes
        Voice* voice = dynamic_cast<Voice*>(synthVoice);
        voice->setEnvelopeSampleRate(sampleRate);
    }
}

void Granulator::setEnvelope(float a, float d, float s, float r)
{
    this->envelopeParams.attack = a;                        // Set the envelope parameters
    this->envelopeParams.decay = d;
    this->envelopeParams.sustain = s;
    this->envelopeParams.release = r;
    for (auto synthVoice : this->voices) {                  // Apply the parameters to all voices envelopes
        Voice* voice = dynamic_cast<Voice*>(synthVoice);
        voice->setEnvelope(&this->envelopeParams);
    }
}
