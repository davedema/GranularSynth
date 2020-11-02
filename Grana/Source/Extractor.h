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
#include <thread>         // std::thread

enum
{
    fftOrder = 11, // 2^11 = 2048 fft size
    fftSize = 1 << fftOrder, // << to binary value
    scopeSize = 256 // graphic bins to be drawn
};

class Extractor {
public:
        
    Extractor();
    ~Extractor();

    void pushSample(float sample);
    void computeSpectrum();
    void setTarget(SpectrumDrawable* s);

    static void fireThread(Extractor* extractor); //fired as a separate thread in PluginProcessor

protected:

    SpectrumDrawable* spectrumDrawable;
    float bins[scopeSize]; // frequency bins to plot
    bool isBlockReady;
     
private:
    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;
    
    

    float input[fftSize]; // input block to transform
    float spectrum[2 * fftSize]; //output spectrum
  
    int write_idx; // writing index - managing a different number of samples per block in processor
};