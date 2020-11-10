/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"

Grain::Grain(int grainDuration, int startPos, bool highreSolution, float freqShift, int envelopeType, float envelopeWidth, int hostRate, int direction) :
    length(grainDuration), startPosition(startPos)
  
{
    this->buffer = new AudioBuffer<float>(FileLoader::getInstance()->getAudioBuffer()->getNumChannels(), this->length);
    this->hilbertTransform = FileLoader::getInstance()->getHilbertTransform();
    this->currentPosition = 0;
    this->finished = false;
    this->lag = 0;

    for (int i = 0; i < FileLoader::getInstance()->getAudioBuffer()->getNumChannels(); i++)
        channelFreqShift(freqShift, i, envelopeType, envelopeWidth, hostRate);

    //If the speed is negative reverse the buffer
    if (direction == -1)
        this->buffer->reverse(0, this->length);
}

Grain::~Grain()
{
    delete buffer;
}

void Grain::channelFreqShift(float freqShift, int channel, int envType, float envWidth, int hostRate)
{
    for (int i = 0; i < length; i++) {//freq shift  --->    ref links
        float phaseInc = freqShift * i / hostRate;
        phaseInc -= floor(phaseInc); //handle phase

        float theta = TWOPI * phaseInc; //angle
        float newValue = hilbertTransform[bufferHilbertIndex(channel, i)] * cos(theta) -
            hilbertTransform[bufferHilbertIndex(channel, i) + 1] * sin(theta); //rotation
        newValue *= GrainEnvelope::getEnvelopeValue(i, envType, this->length, envWidth);
        this->buffer->setSample(channel, i, newValue); //rewrite channel
    }

}

float Grain::getCurrentSample(int channel)
{
    float returnSample = 0;

    if (this->lag == 0) 
    {
        returnSample = buffer->getSample(channel % FileLoader::getInstance()->getAudioBuffer()->getNumChannels(), this->currentPosition);
    }

    return returnSample;
}

void Grain::updateIndex()
{
    if (this->lag == 0)
    {
        this->currentPosition++;
        if (this->currentPosition == this->length) {
            this->finished = true;
        }
    }
    else
    {
        this->lag--;
    }
}

int Grain::bufferHilbertIndex(int channel, int index)
{
   return 2 * (channel * FileLoader::getInstance()->getCeiledLength() + startPosition + index);
}

bool Grain::isFinished()
{
    return this->finished;
}

AudioBuffer<float>* Grain::getBuffer()
{
    return this->buffer;
}

void Grain::setLag(int lag)
{
    this->lag = lag;
}

void Grain::applyCrossFade(int crossfade, bool atStart, int lag)
{
    if (atStart) {     
        crossfade = std::min(crossfade, this->length - 1);
        this->buffer->applyGainRamp(0, crossfade, 0, 1);
    }
    else {
        int position = std::min(this->currentPosition + lag, this->length - 1);
        this->buffer->applyGainRamp(position, this->length - 1 - position, 1, 0);
    }
}

int Grain::remainingLife()
{
    return this->length - this->currentPosition;
}

