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

private: 
    ColourScheme getColourScheme();

    Colour c1{ Colour(37, 44, 70) }; // darkest
    Colour c2{ Colour(98, 102, 150) };
    Colour c3{ Colour(106, 156, 190) };
    Colour c4{ Colour(120, 178, 236) };
    Colour c5{ Colour(237, 238, 243) }; //brightest

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};