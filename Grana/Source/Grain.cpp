/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"

#ifdef __cplusplus 
extern "C" {
#endif
#include "./Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/transforms.h"
#ifdef __cplusplus 
}
#endif


Grain::Grain(int length, int startPos) :
    length(length), startPosition(startPos)
    
{
    fileLoader = FileLoader::getInstance();
    this->numChannels = fileLoader->getAudioBuffer()->getNumChannels();
    envelope = GaussianEnvelope::getInstance();
    hilbertTransform = (double*)malloc(sizeof(double) * numChannels * 2 * this->length); //allocate a transform for every channel
    buffer = processBuffer(); 
    float mainLobeWidth = 0.95; //connect to treestate
    nextOnsetTime = 0;
    playbackRate = 1;
    maxValue = buffer->getMagnitude(0, length);
    
}

Grain::~Grain()
{
    free(hilbertTransform);
    delete buffer;
}

AudioBuffer<float>* Grain::processBuffer()
{
    AudioBuffer<float>* returnBuffer = new AudioBuffer<float>(this->numChannels, this->length);
    returnBuffer->clear();
 
    for (int i = 0; i < this->numChannels; i++) {
        returnBuffer->copyFrom(i, 0, *(fileLoader->getAudioBuffer()), i, this->startPosition, this->length); //copy buffer
        for (int j = 0; j < length; j++) { //apply envelope
            returnBuffer->applyGain(i, j, 1, envelope->currentValue(j));
            hilbertTransform[i * this->length + j] = returnBuffer->getSample(i, j);
            hilbertTransform[i * this->length + j + 1] = 0; //real signal ----> a value every two set to zero
        }
        hilbert(&hilbertTransform[i * this->length], this->length); //hilbertTransform is now the Hilbert transform of the grain
    }
    return returnBuffer;
}

inline float Grain::cubicinterp(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);

    return ((c3 * x + c2) * x + c1) * x + c0;
}

/**void Grain::process(AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, 
    int fileNumSamples, long long int time)
{
    for (int channel = 0; channel < numChannels; ++channel) {
        const float gain = 0; //envelope(time);

        // [1]
        float* channelData = currentBlock.getWritePointer(channel);
        const float* fileData = fileBuffer.getReadPointer(channel % fileBuffer.getNumChannels());

        const float position = (time - onset) * fileLoader->getSampleRate();
        const int iPosition = (int)std::ceil(position);

        // [2]
        const float fracPos = position - iPosition;

        const int readPos = iPosition + startPosition;

        // [3]
        float currentSample = fileData[readPos % fileNumSamples];
        float a = fileData[(readPos - 3) % fileNumSamples];
        float b = fileData[(readPos - 2) % fileNumSamples];
        float c = fileData[(readPos - 1) % fileNumSamples];

        currentSample = cubicinterp(fracPos, a, b, c, currentSample);
        currentSample = currentSample * gain * amp;

        channelData[time % blockNumSamples] += currentSample;
    }
}**/

void Grain::changeEnvelope(EnvType type){

    if (envelope != nullptr)
        delete envelope;

    switch (type) {
    case EnvType::gaussian: envelope = GaussianEnvelope::getInstance();
        break;
    case EnvType::raisedCosineBell: envelope = RaisedCosineBellEnvelope::getInstance();
        break;
    case EnvType::trapezoidal: envelope = TrapezoidalEnvelope::getInstance();
        break;
    default:
        break;
    }
}

int Grain::getLength()
{
    return this->length;
}

float Grain::getSample(int channel, int index)
{
    return this->buffer->getSample(channel, index);
}



int Grain::getNextOnsetTime(){

    return (int)this->length/2; //dummy value using a 50%  constant overlap
}

int Grain::getNumChannels()
{
    return this->numChannels;
}
