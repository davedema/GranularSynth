/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 23 May 2020 4:14:57pm
    Author:  daveg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:

    KnobLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    juce::Label* createSliderTextBox(juce::Slider& slider) override;

private:
    juce::Image img1;
    juce::Font daFont;
};
