/*
  ==============================================================================

    EnvelopeDrawable.h
    Created: 26 Oct 2020 10:33:07pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "./GrainEnvelope.h"
#include "Palette.h"


class EnvelopeDrawable : public Component {
public:
    EnvelopeDrawable();
    ~EnvelopeDrawable();

    void paint(Graphics&) override;
    void resized() override;
    void setType(int envType);
    void setWidth(float envWidth);

private: 
    int envType{ 1 };
    float envWidth{ 0.5 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeDrawable)
};