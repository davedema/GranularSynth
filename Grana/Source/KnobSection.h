/*
  ==============================================================================

    KnobSection.h
    Created: 26 May 2020 1:58:38pm
    Author:  daveg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EnvelopeDrawable.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; //type for slider attachment
constexpr auto NUM_CONTROLS = 3;

//==============================================================================
/*
*/

class KnobSection : public Component, public Slider::Listener
{
public:
    KnobSection();
    ~KnobSection();
    void init(AudioProcessorValueTreeState& apvts);
    void paint(Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void envelopeSelected();
    void widthChanged();

private:
    Slider controls[NUM_CONTROLS];
    Label labels[NUM_CONTROLS];
    std::string controlNames[NUM_CONTROLS] = { "Density", "Grain Size", "Speed" };

    SliderAttachment *attachments[NUM_CONTROLS];
    SliderAttachment * envAttachment;

    ComboBox envelopeList; //list of envelopes
    EnvelopeDrawable envDraw;
    Slider envShape; // shape of the envelope to apply
    Label envShapelab;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobSection)
};