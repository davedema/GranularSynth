/*
  ==============================================================================

    SpectrumDrawable.h
    Created: 26 Oct 2020 11:33:12pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum
{
    fftOrder = 11, // 2^11 = 2048 fft size
    fftSize = 1 << fftOrder, // << to binary value
    scopeSize = 512 // graphic bins to be drawn
};

class SpectrumDrawable : public Component {
public:
    SpectrumDrawable();
    ~SpectrumDrawable();

    void paint(Graphics&) override;
    void resized() override;

private: 
    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;
    
    float input[fftSize]; // input block to transform
    float spectrum[2 * fftSize]; //output spectrum

    float bins[scopeSize]; // frequency bins to plot


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumDrawable)
};