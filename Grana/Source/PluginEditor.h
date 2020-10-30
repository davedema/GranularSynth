/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FileLoader.h"
#include "KnobSection.h"
#include "SpectrumDrawable.h"
#include "FreqTimeDrawable.h"
//==============================================================================
/**
*/
class LaGranaAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::FileDragAndDropTarget, public juce::Button::Listener
{
public:
    LaGranaAudioProcessorEditor (LaGranaAudioProcessor&);
    ~LaGranaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LaGranaAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState* valueTreeState;

    TextButton *loadBtn;
    AudioFormatManager *formatManager;
    AudioTransportSource *transportSource;        

    AudioThumbnailCache* thumbnailCache;
    AudioThumbnail* thumbnail;
    FileLoader *loader;

    Slider filepos;
    Slider sectionsize;
    Label labfilepos;
    Label labsectionsize;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> fileposAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> secsizeAttachment;


    KnobSection controlSection;
    TextButton playStop;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> playAttachment; // button value

    SpectrumDrawable spectrum;
    FreqTimeDrawable xyPlane;

    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintSelected(juce::Graphics& g);

    void loadBtnClicked();

    void thumbnailChanged();

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void envelopeSelected();

    void buttonClicked(Button* button) override;
    void hasChanged();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaGranaAudioProcessorEditor)
};
