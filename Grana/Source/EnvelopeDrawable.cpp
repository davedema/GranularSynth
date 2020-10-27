/*
  ==============================================================================

    EnvelopeDrawable.cpp
    Created: 26 Oct 2020 10:33:07pm
    Author:  amere

  ==============================================================================
*/

#include "EnvelopeDrawable.h"

EnvelopeDrawable::EnvelopeDrawable()
{
    setSize(200, 50);
}

EnvelopeDrawable::~EnvelopeDrawable()
{
}

void EnvelopeDrawable::paint(Graphics&g)
{
    float current_size = GaussianEnvelope::getDuration();
    if (current_size == 0) return;
    g.setColour(juce::Colours::cadetblue);
    //g.drawRect(Rectangle<float>(0, 0, this->getWidth(), this->getHeight()));
    float size_x = this->getWidth();
    float ratio = size_x / current_size;

    Path myPath;
    myPath.startNewSubPath(ratio, this->getHeight() * (1 - GaussianEnvelope::getInstance()->currentValue(1)) + 5);          // move the current position to (10, 10)

    for (int i = 1; i < current_size - 1; i++) {

       myPath.lineTo(i * ratio, this->getHeight() * (1 - GaussianEnvelope::getInstance()->currentValue(i)) + 5);
    }
    g.strokePath(myPath, PathStrokeType(3.0f));

}

void EnvelopeDrawable::resized()
{
}
