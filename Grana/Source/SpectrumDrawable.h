/*
  ==============================================================================

    SpectrumDrawable.h
    Created: 26 Oct 2020 11:33:12pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Palette.h"

class SpectrumDrawable : public Component {
public:
    SpectrumDrawable();
    ~SpectrumDrawable();

    void paint(Graphics&) override;
    void resized() override;
    void drawNextFrame(float* bins, float* freqBins, float measuredShift,float resolution, float averageFreq, float sampleRate);
    void drawGrid(Graphics&);

private: 
    float* currentFrame;
    float* freqBins;
    float measuredShift;
    float averageFrequency;
    float resolution;
    float sampleRate;
    Array<float> verticalLines;
    Array<float> horizontalLines;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumDrawable)
};