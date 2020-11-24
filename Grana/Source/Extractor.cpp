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
    this->currentMaximum = 0;
    this->currentMaximumIndex = 0;
    this->previousMaximumIndex = -1;
    this->totalShift = 0;

    this->isBlockReady = false;
    this->write_idx = 0;
    zeromem(bins, sizeof(bins));
    startTimerHz(60);
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

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f); // skew x-axis onto log scale
        auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f)); //get corresponding index
        auto level = jmap(jlimit(mindB, maxdB, 
            Decibels::gainToDecibels(spectrum[fftDataIndex]) - Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);
        bins[i] = level;

        if (spectrum[fftDataIndex] > this->currentMaximum) {
            this->currentMaximum = spectrum[fftDataIndex];
            this->currentMaximumIndex = fftDataIndex;
        }
    }
}

void Extractor::timerCallback()
{
    if (!this->model->getIsPlaying())
        resetTotal();

    if (isBlockReady) {
        computeSpectrum();
        int increment = (this->currentMaximumIndex - this->previousMaximumIndex) * (model->getSampleRate() / (2 * fftSize));

        if (this->previousMaximumIndex >= 0) 
            this->totalShift += increment;
        else //at the beginning start from the same offset
            this->totalShift += model->getCurrentFrequencyShift();

        this->spectrumDrawable->drawNextFrame(bins, this->totalShift);
        isBlockReady = false;
        this->previousMaximumIndex = this->currentMaximumIndex;
        this->currentMaximum = 0;
        this->currentMaximumIndex = 0;
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
    this->totalShift = 0;
    this->currentMaximum = 0;
    this->currentMaximumIndex = 0;
    this->previousMaximumIndex = -1;
}
