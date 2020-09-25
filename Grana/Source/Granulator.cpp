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
    this->envelope = new ADSR();
    this->envelopeParams = new ADSR::Parameters();
    for (int i; i < VOICES; ++i)                            // Add voices to the synth
        this->addVoice(new Voice(this->envelope));
    /*
    this->addSound(new GrainCloud());                       // Add the sound of the synth (that is the GrainCloud)
    */
}

Granulator::~Granulator()
{
    delete this->envelope;
    delete this->envelopeParams;
    delete &this->voices;
}

void Granulator::setEnvelopeSampleRate(double sampleRate)
{
    this->envelope->setSampleRate(sampleRate);
}

void Granulator::setEnvelope(float a, float d, float s, float r)
{
    // Set the envelope parameters
    this->envelopeParams->attack = a;
    this->envelopeParams->decay = d;
    this->envelopeParams->sustain = s;
    this->envelopeParams->release = r;

    // Apply the parameters to the envelope
    this->envelope->setParameters(*this->envelopeParams);
}
