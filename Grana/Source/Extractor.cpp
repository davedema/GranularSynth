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
    this->isBlockReady = false;
    this->write_idx = 0;
    zeromem(bins, sizeof(bins));
    
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
    }

}


void Extractor::setTarget(SpectrumDrawable* s)
{
    this->spectrumDrawable = s;
}

void Extractor::fireThread(Extractor* extractor)
{
    
    if (!extractor->isBlockReady) //return case
        return;

    MessageManagerLock mml(Thread::getCurrentThread()); //just recommended juce precautions, not really useful here
    while(!mml.lockWasGained())
    {   
        std::cout << "waiting...";
        _sleep(1);
    }

    //begin parallel computing
    extractor->computeSpectrum();
    extractor->spectrumDrawable->drawNextFrame(extractor->bins);
    extractor->isBlockReady = false;
    //end parallel computing
}
