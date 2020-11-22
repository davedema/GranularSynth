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
#include "WaveformDrawable.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; //type for slider attachment
constexpr auto NUM_CONTROLS = 4;

//==============================================================================
/*
*/

class KnobSection : public Component, public Slider::Listener
{
public:
    KnobSection();
    ~KnobSection();
    void init(AudioProcessorValueTreeState& apvts, WaveformDrawable* waveform);
    void paint(Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void envelopeSelected();
    void widthChanged();

private:
    //filepos and section size
    Slider filepos;
    Slider sectionsize;
    Label labfilepos;
    Label labsectionsize;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> fileposAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> secsizeAttachment;

    //grain controls
    Slider controls[NUM_CONTROLS];
    Label labels[NUM_CONTROLS];
    std::string controlNames[NUM_CONTROLS] = { "Density", "Grain Size", "Spread", "Speed" };
    SliderAttachment *attachments[NUM_CONTROLS];

    //envelopes
    ComboBox envelopeList; //list of envelopes
    EnvelopeDrawable envDraw;
    Slider envShape; // shape of the envelope to apply
    Label envShapelab;
    SliderAttachment* widthAttachment;
    ComboBoxParameterAttachment* typeAttachment;

    WaveformDrawable* waveform; // pointer to waveform to repaint it on user's input


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobSection)
};