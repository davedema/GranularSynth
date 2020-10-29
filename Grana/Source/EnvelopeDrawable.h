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

class EnvelopeDrawable : public Component {
public:
    EnvelopeDrawable();
    ~EnvelopeDrawable();

    void paint(Graphics&) override;
    void resized() override;
    void setType(int envType);

private: 
    int envType = 1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeDrawable)
};