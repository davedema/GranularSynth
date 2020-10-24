/*
  ==============================================================================

    Grain.cpp
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/

#include "Grain.h"
#include <cmath>>

#include "./Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/math_const.h"

#ifdef __cplusplus 
extern "C" {
#endif
#include "./Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/transforms.h"

#ifdef __cplusplus 
}
#endif


Grain::Grain(int length, int startPos) :
    length(length), startPosition(startPos), highResolution(false)
    
{
    fileLoader = FileLoader::getInstance();
    sampleRate = fileLoader->getSampleRate();
    this->numChannels = fileLoader->getAudioBuffer()->getNumChannels();
    envelope = GaussianEnvelope::getInstance();
    if (sampleRate / (2 * length) <= 20)                                    //if under JND keep length 
        ceiledLength = pow(2, ceil(log2(length)));                          //zero pad
    else                                                                    //JND as resoulion maximum
        ceiledLength = pow(2, ceil(log2(sampleRate / 2 * 20)));
    //allocate a transform for every channel
    hilbertTransform = (double*)calloc((size_t)(numChannels * (size_t)2 * ceiledLength), sizeof(double));

    buffer = processBuffer(); 
    float mainLobeWidth = 0.95;                                               //connect to treestate
    nextOnsetTime = 0;
    
    maxValue = buffer->getMagnitude(0, length);
}

Grain::Grain(int length, int startPos, bool highreSolution) :
    length(length), startPosition(startPos), highResolution(highreSolution)
{
    fileLoader = FileLoader::getInstance();
    sampleRate = fileLoader->getSampleRate();
    this->numChannels = fileLoader->getAudioBuffer()->getNumChannels();
    envelope = GaussianEnvelope::getInstance();
    ceiledLength = pow(2, ceil(log2(length)));
    if (sampleRate / (2 * ceiledLength) >= 20 && highreSolution)                  //if over JND and high resolution
        ceiledLength = pow(2, ceil(log2(sampleRate / 2 * 20)));                                                    
    //allocate a transform for every channel
    hilbertTransform = (double*)calloc((size_t)(numChannels * (size_t)2 * ceiledLength), sizeof(double));

    buffer = processBuffer();
    integrator = new SimpsonIntegrator(hilbertTransform, sampleRate, ceiledLength, this->numChannels, this->length);
    averageFrequency = integrator->getAverageFrequency();
    averageFrequencies.add(averageFrequency);
    averageTime = integrator->getAverageTime();

    delete integrator;                                                             //useless after
    float mainLobeWidth = 0.95;                                                    //connect to treestate
    nextOnsetTime = 0;

    maxValue = buffer->getMagnitude(0, length);
}


Grain::~Grain()
{
    free(hilbertTransform);
    for (auto buff : freqShiftedGrains)
        delete buff;
    
}

AudioBuffer<float>* Grain::processBuffer()
{
    AudioBuffer<float>* returnBuffer = fileLoader->getAudioBuffer();
    //returnBuffer->clear();
 
    for (int i = 0; i < this->numChannels; i++) {
        for (int j = 0; j < length; j++) { //apply envelope
            //returnBuffer->applyGain(i, j, 1, envelope->currentValue(j));
            if (hilbertTransform != NULL) {
                hilbertTransform[i * 2 * ceiledLength + j * 2] = 
                    fileLoader->getHilbertTransform()[bufferHilbertIndex(i, j)];
                hilbertTransform[i * 2 * ceiledLength + j * 2 + 1] = 
                    fileLoader->getHilbertTransform()[bufferHilbertIndex(i, j) + 1]; //complex signal
            }
        }

    }
    integrator = new SimpsonIntegrator(hilbertTransform, sampleRate, ceiledLength, this->numChannels, this->length);
    averageFrequency = integrator->getAverageFrequency();
    averageFrequencies.add(averageFrequency);
    averageTime = integrator->getAverageTime();

    delete integrator;                                                        //useless after
    return returnBuffer;
}

void Grain::equalTemperament()
{
    float differenceFromALow = 55 - this->averageFrequency;
    float freqRange = 110;
    int octavestep = 0;

    jassert(hilbertTransform != nullptr);

    if (hilbertTransform != nullptr) {
        for (int octave = 0; octave < 6; octave++) {

            for (int note = 0; note < 12; note++) {
                float fShift = differenceFromALow + octavestep + note * freqRange / 12;
                freqShiftedGrains.add(freqShift(fShift));
                averageFrequencies.add(averageFrequencies.getLast() + fShift);
            }

            octavestep += freqRange;
            freqRange *= 2;          
        }
        this->temperament = Temperament::equalTemperament;
    }
}

void Grain::channelFreqShift(AudioBuffer<float>* buffer, float freqShift, int channel)
{
    for (int i = 0; i < length; i++) {//freq shift  --->    ref links
        float phaseInc = freqShift * i / this->fileLoader->getSampleRate();
        if (phaseInc >= 1) //handle phase
            phaseInc -= 1.0f;
        float theta = TWOPI * phaseInc; //angle
        float newValue = this->getBuffer()->getSample(channel, bufferIndex(channel, i)) * cos(theta) -
            hilbertTransform[this->ceiledLength * channel + i + 1] * sin(theta); //rotation
        newValue *= envelope->currentValue(i);
        buffer->setSample(channel, i, newValue); //rewrite channel
    }

}

AudioBuffer<float>* Grain::freqShift(float freqshift)
{
    AudioBuffer<float>* returnBuffer = new AudioBuffer<float>(*this->buffer);
    for (int i = 0; i < this->numChannels; i++)
        channelFreqShift(returnBuffer, freqshift, i);
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

int Grain::getCeiledLength()
{
    return this->ceiledLength;
}

int Grain::bufferHilbertIndex(int channel, int index)
{
   return 2 * channel * fileLoader->getCeiledLength() + startPosition + index * 2;
}

int Grain::bufferIndex(int channel, int index)
{
    return channel * buffer->getNumSamples() + startPosition + index;
}



int Grain::getNextOnsetTime(){

    return (int)this->length/2; //dummy value using a 50%  constant overlap
}

int Grain::getNumChannels()
{
    return this->numChannels;
}

float Grain::getMaxValue()
{
    return this->maxValue;
}

int Grain::getStartPosition()
{
    return this->startPosition;
}

float Grain::getAverageFrequency()
{
    return this->averageFrequency;
}

float Grain::getAverageTime()
{
    return this->averageTime;
}

GrainEnvelope* Grain::getEnvelope()
{
    return this->envelope;
}

AudioBuffer<float>* Grain::getBuffer()
{
    return this->buffer;
}

double* Grain::getHilbertTransform()
{
    return this->hilbertTransform;
}

Array<AudioBuffer<float>*> Grain::getFreqShiftedGrains()
{
    return this->freqShiftedGrains;
}

SimpsonIntegrator::SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels, int notCeiledLength) :
    samplingFrequency(samplingFrequency), length(length), numChannels(numChannels), notCeiledLength(notCeiledLength)
{
    computeAverageFrequency(hilbertTransform);
    computeAverageTime(hilbertTransform);
}

SimpsonIntegrator::SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels, int notCeiledLength, float freqshift) :
    samplingFrequency(samplingFrequency), length(length), numChannels(numChannels), notCeiledLength(notCeiledLength)
{
    computeAverageFrequency(hilbertTransform, freqshift);
    computeAverageTime(hilbertTransform, freqshift);
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

    //allocate a spectrum for every channel
    hilbertSpectrum = (double*)calloc((size_t)(numChannels * (size_t)2 * length), sizeof(double));

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
                float normIncrement = (float)(pow(hilbertSpectrum[i * 2 * length + j * 2], 2) + 
                    pow(hilbertSpectrum[i * 2 * length + j * 2 + 1], 2));
                float averageFreqIncrement = normIncrement * step * i;
                if (j < notCeiledLength) {//if under original length apply envelope
                    averageFreqIncrement *= GaussianEnvelope::getInstance()->currentValue(j);
                    normIncrement *= GaussianEnvelope::getInstance()->currentValue(j);
                }
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
            norm *= step / 3;
            averageFrequency *= step / 3;
            averageFrequency /= norm;   //normalize
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
                if (j < notCeiledLength) { //if under original length apply envelope
                    averageTimeIncrement *= GaussianEnvelope::getInstance()->currentValue(j);
                    normIncrement *= GaussianEnvelope::getInstance()->currentValue(j);
                }

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
            norm *= step / 3;
            averageTime *= step / 3; 
            averageTime /= norm;  //normalize 
            totalAverageTime += averageTime;
        }
        totalAverageTime /= numChannels; //average of channels (probably overkilling here)
        this->averageTime = totalAverageTime;
    }
}

void SimpsonIntegrator::computeAverageFrequency(double* hilbertTransform, float freqShift)
{
    float nyquist = samplingFrequency / 2;
    float step = nyquist / length; //freq resolution
    float totalAverageFrequency = 0;

    //allocate a spectrum for every channel
    hilbertSpectrum = (double*)calloc((size_t)(numChannels * (size_t)2 * length), sizeof(double));

    for (int i = 0; i < this->numChannels; i++) { //loop over channels

        float norm = 0;
        float averageFrequency = 0;
        if (hilbertSpectrum != NULL) {
            for (int j = 0; j < length; j++) { //copy transform
                if (hilbertSpectrum != NULL) {
                    float phaseInc = freqShift * j / this->samplingFrequency;
                    float theta = TWOPI * phaseInc; //angle
                    float newValueX = hilbertTransform[this->length * i + j] * cos(theta) -
                        hilbertTransform[this->length * i + j + 1] * sin(theta); //rotation
                    float newValueY = hilbertTransform[this->length * i + j] * sin(theta) +
                        hilbertTransform[this->length * i + j + 1] * cos(theta); //rotation
                    hilbertSpectrum[i * 2 * length + j * 2] = newValueX;
                    hilbertSpectrum[i * 2 * length + j * 2 + 1] = newValueY;
                }

            }

            if (hilbertSpectrum != NULL) {
                fft_dif(&hilbertSpectrum[i * (int)2 * length], length); // compute fft
                bitrev_permute(&hilbertSpectrum[i * 2 * length], length); // bit-revert output (Smithsonian implementation)
            }

            for (int j = 0; j < length; j++) { //integrate frequency
                float normIncrement = (float)(pow(hilbertSpectrum[i * 2 * length + j * 2], 2) + 
                    pow(hilbertSpectrum[i * 2 * length + j * 2 + 1], 2));
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
            norm *= step / 3;
            averageFrequency *= step / 3;
            averageFrequency /= norm;   //normalize
            totalAverageFrequency += averageFrequency;
        }
        totalAverageFrequency /= numChannels; //average of channels (probably overkilling here)
        this->averageFrequency = totalAverageFrequency;
    }
}

void SimpsonIntegrator::computeAverageTime(double* hilbertTransform, float freqShift)
{
    float step = 1 / samplingFrequency; //time resolution
    float totalAverageTime = 0;

    if (hilbertTransform != NULL) {
        for (int i = 0; i < this->numChannels; i++) { //loop over channels

            float norm = 0;
            float averageTime = 0;

            for (int j = 0; j < length; j++) { //integrate time
                float phaseInc = freqShift * j / this->samplingFrequency;
                float theta = TWOPI * phaseInc; //angle
                float newValueX = hilbertTransform[this->length * i + j] * cos(theta) -
                    hilbertTransform[this->length * i + j + 1] * sin(theta); //rotation
                float newValueY = hilbertTransform[this->length * i + j] * sin(theta) +
                    hilbertTransform[this->length * i + j + 1] * cos(theta); //rotation
                float normIncrement = (float)pow(newValueX, 2) + pow(newValueY, 2);
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
            norm *= step / 3;
            averageTime *= step / 3;
            averageTime /= norm;  //normalize 
            totalAverageTime += averageTime;
        }
        totalAverageTime /= numChannels; //average of channels (probably overkilling here)
        this->averageTime = totalAverageTime;
    }
}

