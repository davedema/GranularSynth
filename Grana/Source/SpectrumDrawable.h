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
    void drawNextFrame(float* bins, float measuredShift, float resolution, float originalInstantaneousFreq, float averageFreq);
    void drawGrid(Graphics&);

private: 
    float* currentFrame;
    float measuredShift;
    float resolution;
    float originalInstantaneousFrequency;
    float averageFrequency;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumDrawable)
};