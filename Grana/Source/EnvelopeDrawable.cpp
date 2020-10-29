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
    g.setColour(juce::Colours::cadetblue);
    //g.drawRect(Rectangle<float>(0, 0, this->getWidth(), this->getHeight()));
    float size_x = this->getWidth();
    if (size_x == 0) return;

    Path myPath;
    myPath.startNewSubPath(0, GrainEnvelope::getEnvelopeValue(0, 1, size_x, 1));         // move the current position to (10, 10)

    for (int i = 1; i < size_x - 1; i++) {

       myPath.lineTo(i, this->getHeight() * (1 - GrainEnvelope::getEnvelopeValue(i, this->envType, size_x, 1)));
    }
    g.strokePath(myPath, PathStrokeType(3.0f));

}

void EnvelopeDrawable::resized()
{
}

void EnvelopeDrawable::setType(int envType)
{
    this->envType = envType;
}
