/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FileLoader.h"
#include "WaveformDrawable.h"
#include "FileSection.h"
#include "KnobSection.h"
#include "SpectrumDrawable.h"
#include "FreqTimeDrawable.h"
//==============================================================================
/**
*/

class LaGranaAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    LaGranaAudioProcessorEditor(LaGranaAudioProcessor&);
    ~LaGranaAudioProcessorEditor() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LaGranaAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState* valueTreeState;

    Slider masterGain;
    Label masterGainlab;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> masterGainAttachment; // gain att

    WaveformDrawable waveform;
    FileSection fileSection;
    KnobSection controlSection;

    SpectrumDrawable spectrum;
    FreqTimeDrawable xyPlane;

    Colour c1 { Colour(37, 44, 70) };
    Colour c2 { Colour(98, 102, 150) };
    Colour c3 { Colour(106, 156, 190) };
    Colour c4 { Colour(120, 178, 236) };
    Colour c5{ Colour(151, 158, 243) };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaGranaAudioProcessorEditor)
};
