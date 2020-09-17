/*
  ==============================================================================

    KnobLookAndFeel.cpp
    Created: 23 May 2020 4:14:57pm
    Author:  daveg

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KnobLookAndFeel.h"



//==============================================================================
KnobLookAndFeel::KnobLookAndFeel()
{
    img1 = ImageCache::getFromMemory(BinaryData::Carbon_png, BinaryData::Carbon_pngSize);
    //daFont = Typeface::createSystemTypefaceFor(BinaryData::Liquid_ttf,BinaryData::Liquid_ttfSize);
    daFont = juce::Typeface::createSystemTypefaceFor(BinaryData::Brotherline_ttf, BinaryData::Brotherline_ttfSize);
    //daFont = Typeface::createSystemTypefaceFor(BinaryData::riesling_ttf, BinaryData::riesling_ttfSize);
}

//==============================================================================


void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if (img1.isValid())
    {
        auto rotation = sliderPos * (rotaryEndAngle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle);
        const int frames = img1.getHeight() / img1.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = jmin(width / 2.0f, height / 2.0f) - 10;
        const float centerX = x + width * 0.53f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;

        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.drawImage(img1, (int)rx, (int)ry, 2 * (int)radius, 2 * (int)radius,
            0, frameId * img1.getWidth(), img1.getWidth(), img1.getWidth());

        float startY = height / 2 + rw * std::cos(rotaryStartAngle);
        float endX = width / 2 + radius * std::sin(rotaryEndAngle);


        juce::ColourGradient* gradient = new juce::ColourGradient(juce::Colours::grey.brighter(0.2), rx, y, juce::Colours::aliceblue,
            endX, startY, false);
        gradient->clearColours();
        gradient->addColour(0, juce::Colours::deepskyblue.darker(0.9));
        gradient->addColour(0.3, juce::Colours::deepskyblue.darker(0.9));
        gradient->addColour(0.6, juce::Colours::crimson.darker(0.3));
        gradient->addColour(0.9, juce::Colours::crimson.darker(0.8));

        juce::Path filledArc;
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle * 0.98, angle * 1.02, 0.8);
        g.setGradientFill(*gradient);
        g.fillPath(filledArc);

        delete gradient;
    }
    else
    {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f,
            0.5f * height, width * textPpercent, 0.5f * height);

        g.setColour(juce::Colours::white);

        g.drawFittedText(juce::String("No Image"), text_bounds.getSmallestIntegerContainer(),
            juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}



Label* KnobLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    Label* l = LookAndFeel_V3::createSliderTextBox(slider);
    l->setColour(Label::textColourId, Colour::fromRGB(243, 243, 245).darker(0.3));
    daFont.setHeight(22);
    l->setFont(daFont);
    l->setColour(Label::outlineColourId, Colours::transparentWhite);
    l->setColour(Label::outlineWhenEditingColourId, Colours::transparentWhite);
    return l;
}