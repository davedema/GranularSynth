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
    g.fillAll(Colours::black);

    g.setOpacity(1.0f);
    g.setColour(Colours::aliceblue);

    if (this->currentFrame != nullptr)
    {
        int size = 256;
        for (int i = 1; i < size; ++i)
        {
            auto width = getLocalBounds().getWidth();
            auto height = getLocalBounds().getHeight();

            g.drawLine({ (float)jmap(i - 1, 0, size - 1, 0, width),
                                  jmap(*(currentFrame + i), 0.0f, 1.0f, (float)height, 0.0f),
                          (float)jmap(i,     0, size - 1, 0, width),
                                  jmap(*(currentFrame + i - 1) ,     0.0f, 1.0f, (float)height, 0.0f) });
        }


    }
}

void SpectrumDrawable::resized()
{
}

void SpectrumDrawable::drawNextFrame(float* bins)
{
    currentFrame = bins;
    repaint();
}

