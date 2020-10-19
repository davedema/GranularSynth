/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"
#include <cmath>>

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
    ceiledLength = pow(2, ceil(log2(length)));
    hilbertTransform = (double*)calloc((size_t)(numChannels * (size_t)2 * ceiledLength), sizeof(double)); //allocate a transform for every channel

    buffer = processBuffer(); 
    integrator = new SimpsonIntegrator(hilbertTransform, fileLoader->getSampleRate(), ceiledLength, this->numChannels);
    averageFrequency = integrator->getAverageFrequency();
    averageTime = integrator->getAverageTime();

    delete integrator; //useless after
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
            if (hilbertTransform != NULL) {
                hilbertTransform[i * 2 * ceiledLength + j * 2] = returnBuffer->getSample(i, j);
                hilbertTransform[i * 2 * ceiledLength + j * 2 + 1] = 0; //real signal ----> a value every two set to zero
            }
        }

        if(hilbertTransform != NULL)
            hilbert(&hilbertTransform[i * ceiledLength], ceiledLength); //hilbertTransform is now the Hilbert transform of the grain

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

SimpsonIntegrator::SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels) :
    samplingFrequency(samplingFrequency), length(length), numChannels(numChannels)
{
    computeAverageFrequency(hilbertTransform);
    computeAverageTime(hilbertTransform);
}

SimpsonIntegrator::~SimpsonIntegrator()
{
    free(hilbertSpectrum);
}

float SimpsonIntegrator::getAverageFrequency()
{
    return this->averageFrequency;
}

float SimpsonIntegrator::getAverageTime()
{
    return this->averageTime;
}

void SimpsonIntegrator::computeAverageFrequency(double* hilbertTransform)
{

    float nyquist = samplingFrequency / 2;
    float step = nyquist / length; //freq resolution
    float totalAverageFrequency = 0;

    hilbertSpectrum = (double*)calloc((size_t)(numChannels * (size_t)2 * length), sizeof(double)); //allocate a spectrum for every channel

    for (int i = 0; i < this->numChannels; i++) { //loop over channels

        float norm = 0;
        float averageFrequency = 0;
        if (hilbertSpectrum != NULL) {
            for (int j = 0; j < length; j++) { //copy transform
                if (hilbertSpectrum != NULL) {
                    hilbertSpectrum[i * 2 * length + j * 2] = hilbertTransform[i * 2 * length + j * 2];
                    hilbertSpectrum[i * 2 * length + j * 2 + 1] = hilbertTransform[i * 2 * length + j * 2 + 1];
                }

            }

            if (hilbertSpectrum != NULL) {
                fft_dif(&hilbertSpectrum[i * (int)2 * length], length); // compute fft
                bitrev_permute(&hilbertSpectrum[i * 2 * length], length); // bit-revert output (Smithsonian implementation)
            }

            for (int j = 0; j < length; j++) { //integrate frequency
                float normIncrement = (float)(pow(hilbertSpectrum[i * 2 * length + j * 2], 2) + pow(hilbertSpectrum[i * 2 * length + j * 2 + 1], 2));
                float averageFreqIncrement = normIncrement * step * i;

                //simpson rule
                if (j == 0 || j == length - 1) {
                    norm += normIncrement;
                    averageFrequency += averageFreqIncrement;
                }
                else if (j % 2 == 0) {
                    norm += 2 * normIncrement;
                    averageFrequency += 2 * averageFreqIncrement;
                }
                else {
                    norm += 4 * normIncrement;
                    averageFrequency += 4 * averageFreqIncrement;
                }
            }
            averageFrequency *= step / (3 * norm); //normalize and simpson rule
            totalAverageFrequency += averageFrequency;
        }
        totalAverageFrequency /= numChannels; //average of channels (probably overkilling here)
        this->averageFrequency = totalAverageFrequency;
    }
}

void SimpsonIntegrator::computeAverageTime(double* hilbertTransform)
{
    float step = 1 / samplingFrequency; //time resolution
    float totalAverageTime = 0;

    if (hilbertTransform != NULL) {
        for (int i = 0; i < this->numChannels; i++) { //loop over channels

            float norm = 0;
            float averageTime = 0;

            for (int j = 0; j < length; j++) { //integrate time
                float normIncrement = (float)(pow(hilbertTransform[i * 2 * length + j * 2], 2) +
                    pow(hilbertTransform[i * 2 * length + j * 2 + 1], 2));
                float averageTimeIncrement = normIncrement * step * i;

                //simpson rule
                if (j == 0 || j == length - 1) {
                    norm += normIncrement;
                    averageTime += averageTimeIncrement;
                }
                else if (j % 2 == 0) {
                    norm += 2 * normIncrement;
                    averageTime += 2 * averageTimeIncrement;
                }
                else {
                    norm += 4 * normIncrement;
                    averageTime += 4 * averageTimeIncrement;
                }
            }
            averageTime += step / (3 * norm); //normalize and simpson rule
            totalAverageTime += averageTime;
        }
        totalAverageTime /= numChannels; //average of channels (probably overkilling here)
        this->averageTime = totalAverageTime;
    }
}

