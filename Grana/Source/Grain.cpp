/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"

Grain::Grain(int grainDuration, int startPos, bool highreSolution, float freqShift, int envelopeType, float envelopeWidth, int hostRate) :
    length(grainDuration), startPosition(startPos)
  
{
    this->buffer = new AudioBuffer<float>(FileLoader::getInstance()->getAudioBuffer()->getNumChannels(), this->length);
    this->hilbertTransform = FileLoader::getInstance()->getHilbertTransform();
    this->currentPosition = 0;
    this->finished = false;

    //Computing the frequency shifted buffer
    ceiledLength = pow(2, ceil(log2(length)));
    if (FileLoader::getInstance()->getSampleRate() / (2 * ceiledLength) >= 20 && highreSolution)                  //if over JND and high resolution
        ceiledLength = pow(2, ceil(log2(FileLoader::getInstance()->getSampleRate() / 2 * 20)));                                                    
    this->integrator = new SimpsonIntegrator(hilbertTransform, 
                                             FileLoader::getInstance()->getSampleRate(),
                                             ceiledLength, 
                                             FileLoader::getInstance()->getAudioBuffer()->getNumChannels(), 
                                             this->length);
    this->averageFrequency = this->integrator->getAverageFrequency();
    delete integrator;                                                             //useless after
    for (int i = 0; i < FileLoader::getInstance()->getAudioBuffer()->getNumChannels(); i++)
        channelFreqShift(freqShift, i, envelopeType, envelopeWidth, hostRate);
}

Grain::~Grain()
{
    delete buffer;
}

void Grain::channelFreqShift(float freqShift, int channel, int envType, float envWidth, int hostRate)
{
    for (int i = 0; i < length; i++) {//freq shift  --->    ref links
        float phaseInc = freqShift * i / hostRate;
        if (phaseInc > 1) //handle phase
            while(phaseInc > 1)
                phaseInc -= 1;
        else if (phaseInc < -1)
            while(phaseInc < -1)
                phaseInc += 1;

        jassert(abs(phaseInc) <= 1);

        float theta = TWOPI * phaseInc; //angle
        float newValue = hilbertTransform[bufferHilbertIndex(channel, i)] * cos(theta) -
            hilbertTransform[bufferHilbertIndex(channel, i) + 1] * sin(theta); //rotation
        newValue *= GrainEnvelope::getEnvelopeValue(i, envType, this->length, envWidth);
        this->buffer->setSample(channel, i, newValue); //rewrite channel
    }

}

float Grain::getCurrentSample(int channel)
{
    return buffer->getSample(channel % FileLoader::getInstance()->getAudioBuffer()->getNumChannels(), this->currentPosition);
}

void Grain::updateIndex()
{
    this->currentPosition++;
    if (this->currentPosition == this->length) {
        this->finished = true;
    }
}

int Grain::bufferHilbertIndex(int channel, int index)
{
   return 2 * (channel * FileLoader::getInstance()->getCeiledLength() + startPosition + index);
}

float Grain::getAverageFrequency()
{
    return this->averageFrequency;
}

bool Grain::isFinished()
{
    return this->finished;
}

AudioBuffer<float>* Grain::getBuffer()
{
    return this->buffer;
}

