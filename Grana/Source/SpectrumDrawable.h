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

private: 
    void drawFrame();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumDrawable)
};