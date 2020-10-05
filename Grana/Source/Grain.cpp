/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"


Grain::Grain(int length, int startPos) :
    length(length), startPosition(startPos)
    
{
    fileLoader = FileLoader::getInstance();
    buffer = processBuffer(); 
    float mainLobeWidth = 0.95; //connect to treestate
    playbackRate = 1;
    
}

AudioBuffer<float>* Grain::processBuffer()
{
    AudioBuffer<float>* returnBuffer = new AudioBuffer<float>(2, this->length);
    returnBuffer->copyFrom(0, this->startPosition, fileLoader->getAudioBuffer()->getReadPointer(0), 0, this->length); //copy buffer
    returnBuffer->copyFrom(1, this->startPosition, fileLoader->getAudioBuffer()->getReadPointer(1), 1, this->length);
    for (int i = 0; i < length; i++) { //apply envelope
        *(returnBuffer->getWritePointer(0, i)) *= envelope->currentValue(i); //deferentiating to access values
        *(returnBuffer->getWritePointer(1, i)) *= envelope->currentValue(i);
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
