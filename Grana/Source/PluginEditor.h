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

    WaveformDrawable waveform;
    FileSection fileSection;
    KnobSection controlSection;

    SpectrumDrawable spectrum;
    FreqTimeDrawable xyPlane;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaGranaAudioProcessorEditor)
};
