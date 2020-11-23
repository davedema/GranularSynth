/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 23 Nov 2020 5:56:18pm
    Author:  amere

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColourScheme(getColourScheme());
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(Graphics&g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider&)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 15.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(c1);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(c2);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    Path p;
    auto pointerLength = radius * 0.3f;
    auto pointerThickness = 8.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(c4);
    g.fillPath(p);
}

void CustomLookAndFeel::drawButtonBackground(Graphics& g,
    Button& button,
    const Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
        auto cornerSize = 6.0f;
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

        auto baseColour = c3.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = backgroundColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

        g.setColour(baseColour);

        auto flatOnLeft = button.isConnectedOnLeft();
        auto flatOnRight = button.isConnectedOnRight();
        auto flatOnTop = button.isConnectedOnTop();
        auto flatOnBottom = button.isConnectedOnBottom();

        if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
        {
            Path path;
            path.addRoundedRectangle(bounds.getX(), bounds.getY(),
                bounds.getWidth(), bounds.getHeight(),
                cornerSize, cornerSize,
                !(flatOnLeft || flatOnTop),
                !(flatOnRight || flatOnTop),
                !(flatOnLeft || flatOnBottom),
                !(flatOnRight || flatOnBottom));

            g.fillPath(path);

            g.setColour(c3);
            g.strokePath(path, PathStrokeType(1.0f));
        }
        else
        {
            g.fillRoundedRectangle(bounds, cornerSize);

            g.setColour(c2);
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }
    
}

LookAndFeel_V4::ColourScheme CustomLookAndFeel::getColourScheme()
{
    return {
        0xff151928, //windowbg
        0xff252C46, // widget bg
        //0xff626696, 
        0xff3F4C78, // menu bg
        0xff78B2EC,  //outline
        0xffEDEEFD,  //defualt text
        0xff6A9CBE, // defautkl fill
        0xf788B2EC, //highlighted text
        0xEDEEFD, //menu text 
        0xff6A9CBE // numColours
    };
}
