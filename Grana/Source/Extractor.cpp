/*
  ==============================================================================

    Extractor.cpp
    Created: 27 Oct 2020 10:09:50am
    Author:  amere

  ==============================================================================
*/

#include "Extractor.h"

Extractor::Extractor():forwardFFT(fftOrder), window(fftSize, dsp::WindowingFunction<float>::hamming)
{
    this->model = nullptr;
    this->averageFrequency = 0;

    this->isBlockReady = false;
    this->write_idx = 0;
    zeromem(bins, sizeof(bins));
    startTimerHz(60);
    zeromem(freqBins, sizeof(freqBins));

}

Extractor::~Extractor()
{
    
}

void Extractor::pushSample(float sample)
{
    if (write_idx == fftSize){
        if (!isBlockReady)
        {
            zeromem(spectrum, sizeof(spectrum));
            memcpy(spectrum, input, sizeof(input));
            isBlockReady = true;
        }

        write_idx = 0;
    }

    input[write_idx] = sample;
    write_idx++;
}

void Extractor::computeSpectrum()
{
    window.multiplyWithWindowingTable(spectrum, fftSize); // window the frame
    forwardFFT.performFrequencyOnlyForwardTransform(spectrum); //perform fft on the real part
    auto mindB = -100.0f;
    auto maxdB = 0.0f;
    auto overallMax = juce::findMaximum(spectrum, fftSize);
    int i = 0;
    while (spectrum[i] < overallMax)
        i++;

    float averageFreq = 0, norm = 0;
    float resolution = model->getSampleRate() / (float)fftSize;

    for (int i = 0; i < fftSize/2; i++) { //average frequency over the magnitude spectrum
        averageFreq += i * resolution * spectrum[i];
        norm += spectrum[i];
    }
    if(norm != 0)
        averageFreq /= norm;
    this->averageFrequency = averageFreq;

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::pow(10, std::log10(1.0f - (float)(i+1) / (float)scopeSize)*0.2f); // skew x-axis onto log scale
         auto fftIndex = jlimit(1, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f)); //get corresponding index

        auto level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(spectrum[fftIndex]) - Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);
        bins[i] = level;
        this->freqBins[i] = fftIndex * resolution ;
        
    }
    this->freqBins[scopeSize - 1] = fftSize / 2 * resolution;
}

void Extractor::timerCallback()
{
    if (!this->model->getIsPlaying()) {
        resetTotal(); //reset logic if stopped
        this->spectrumDrawable->drawNextFrame(bins,this->freqBins, model->getSampleRate() / fftSize, this->averageFrequency, this->model->getSampleRate());
        return;
    }

    if (isBlockReady) {
        computeSpectrum();
        this->spectrumDrawable->drawNextFrame(bins, this->freqBins, model->getSampleRate() / fftSize, this->averageFrequency, this->model->getSampleRate());
        isBlockReady = false;
    }
}

void Extractor::setTarget(SpectrumDrawable* s)
{
    this->spectrumDrawable = s;
}

void Extractor::setModel(Model* model)
{
    this->model = model;
}

void Extractor::resetTotal()
{
    this->averageFrequency = 0;
}
