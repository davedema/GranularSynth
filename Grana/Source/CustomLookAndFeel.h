/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 23 Nov 2020 5:32:34pm
    Author:  amere

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public LookAndFeel_V4 {
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();

    void drawRotarySlider(Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider&) override;
    void drawButtonBackground(Graphics& g,
                                Button& button,
                                const Colour& backgroundColour,
                                bool shouldDrawButtonAsHighlighted,
                                bool shouldDrawButtonAsDown) override;

    LookAndFeel_V4::ColourScheme getColourScheme();

private: 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};