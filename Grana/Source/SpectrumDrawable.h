/*
  ==============================================================================

    SpectrumDrawable.h
    Created: 26 Oct 2020 11:33:12pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SpectrumDrawable : public Component {
public:
    SpectrumDrawable();
    ~SpectrumDrawable();

    void paint(Graphics&) override;
    void resized() override;
    void drawNextFrame(float* bins);
    void drawGrid(Graphics&);

private: 
    float* currentFrame;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumDrawable)
};