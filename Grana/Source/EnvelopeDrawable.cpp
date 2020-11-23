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
    g.setColour(Colour(37, 44, 70));
    //g.drawRect(Rectangle<float>(0, 0, this->getWidth(), this->getHeight()));
    float size_x = this->getWidth();
    if (size_x == 0) return;

    Path myPath;
    myPath.startNewSubPath(0, this->getHeight() * (1 - GrainEnvelope::getEnvelopeValue(0, this->envType, size_x, this->envWidth)) + 5);

    for (int i = 1; i < size_x - 1; i++) {

       myPath.lineTo(i, this->getHeight() * (1 - GrainEnvelope::getEnvelopeValue(i, this->envType, size_x, this->envWidth)) + 5);
    }
    myPath.applyTransform(AffineTransform::scale(1.0f, 0.75f));
    g.strokePath(myPath, PathStrokeType(3.0f));

}

void EnvelopeDrawable::resized()
{
}

void EnvelopeDrawable::setType(int envType)
{
    this->envType = envType;
}

void EnvelopeDrawable::setWidth(float envWidth)
{
    this->envWidth = envWidth;
}
