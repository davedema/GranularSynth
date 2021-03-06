/*
  ==============================================================================

    Extractor.h
    Created: 27 Oct 2020 10:09:50am
    Author:  amere
    // class managing audio analysis and processing for outputting spectrum / features

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "./SpectrumDrawable.h"
#include "Model.h"

enum
{
    fftOrder = 11, // 2^11 = 2048 fft size
    fftSize = 1 << fftOrder, // << to binary value
    scopeSize = 256 // graphic bins to be drawn
};

class Extractor: public Timer {
public:
        
    Extractor();
    ~Extractor();

    void pushSample(float sample);
    void computeSpectrum();
    void timerCallback() override;
    void setTarget(SpectrumDrawable* s);
    void setModel(Model* model);

    void resetTotal(); //reset shift measurement logic

      
private:
    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;
    SpectrumDrawable * spectrumDrawable;

    float input[fftSize]; // input block to transform
    float spectrum[2 * fftSize]; //output spectrum
    float bins[scopeSize]; // frequency bins to plot
    float freqBins[scopeSize];

    bool isBlockReady;
    int write_idx; // writing index - managing a different number of samples per block in processor

    float averageFrequency;       //frequency averaged over the magnitude spectrum

    Model* model;
};