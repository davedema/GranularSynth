/*
  ==============================================================================

    SimpsonIntegrator.cpp
    Created: 27 Oct 2020 2:34:24pm
    Author:  gabri

  ==============================================================================
*/

#include "SimpsonIntegrator.h"

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
                   /* averageFreqIncrement *= GaussianEnvelope::getInstance()->currentValue(j);
                    normIncrement *= GaussianEnvelope::getInstance()->currentValue(j); */
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
    /*
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
    */
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

