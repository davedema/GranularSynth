/*
  ==============================================================================

    SpectrumDrawable.cpp
    Created: 26 Oct 2020 11:33:12pm
    Author:  amere

  ==============================================================================
*/

#include "SpectrumDrawable.h"

SpectrumDrawable::SpectrumDrawable()
{
    setSize(200, 100);
    currentFrame = nullptr;
}

SpectrumDrawable::~SpectrumDrawable()
{
}

void SpectrumDrawable::paint(Graphics& g)
{
    g.fillAll(Colour(ColourPalette::drawbox));

    g.setColour(Colour(ColourPalette::bright_component));

    if (this->currentFrame != nullptr)
    {
        auto margin = 5;
        auto height = getLocalBounds().getHeight() - margin;
        auto width = getLocalBounds().getWidth() - margin;

        Path myPath;
        myPath.startNewSubPath(margin, getHeight() - margin);
        int size = 256;
        for (int i = 0; i < size / 2; ++i)
        {
            myPath.quadraticTo(Point<float>((float)jmap(2 * i, 0, size, margin, width - margin),
                jmap(*(currentFrame + 2 * i), 0.0f, 1.0f, (float)height - margin, 0.0f)),
                Point<float>((float)jmap(2 * i + 1, 0, size, margin, width - margin),
                    jmap(*(currentFrame + 2 * i + 1), 0.0f, 1.0f, (float)height - margin, 0.0f)));
        }
        myPath.lineTo(getWidth() - margin, getHeight() - margin);
        g.strokePath(myPath, PathStrokeType(3.0f));
        
        Colour base = Colour(ColourPalette::numbers);
        ColourGradient* gradient = new ColourGradient(Colour(ColourPalette::bright_component), 0, getHeight(), Colour(ColourPalette::numbers), 0, 0, false);
        gradient->clearColours();
        gradient->addColour(0, base.darker().darker());
        gradient->addColour(0.2, base.darker());
        gradient->addColour(0.4, base);
        gradient->addColour(0.7, base.brighter());

        g.setGradientFill(*gradient);

        g.fillPath(myPath);

        delete gradient;

    }  
    this->drawGrid(g);

}

void SpectrumDrawable::resized()
{
}

void SpectrumDrawable::drawNextFrame(float* bins, float measuredShift)
{
    currentFrame = bins;
    repaint();
}

 
void SpectrumDrawable::drawGrid(Graphics& g)
{
    auto margin = 5.0f;
  Line<float> freq_ax(Point<float>(margin, (float)getHeight() - margin),
                        Point<float>((float)getWidth() - margin, (float)getHeight() - margin));

  Line<float> mag_ax(Point<float>(margin, (float)getHeight() - margin),
      Point<float>(margin, margin));
    g.setColour(Colours::white);
    g.drawLine(freq_ax); 
    g.drawLine(mag_ax);
}