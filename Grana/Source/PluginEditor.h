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
#include "FileSection.h"

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

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LaGranaAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState* valueTreeState;

    juce::TextButton *loadBtn;
    juce::AudioFormatManager *formatManager;
    juce::AudioTransportSource *transportSource;        

    juce::AudioThumbnailCache* thumbnailCache;                  // [1]
    juce::AudioThumbnail* thumbnail;                            // [2]
    FileLoader *loader;

    /*KnobSection* grainSection;
    KnobSection* fileSection;*/
    Slider filepos;
    Label labfilepos;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> fileposAttachment;

    KnobSection controlSection;
    FileSection fileSection;
    ComboBox* envelopeList;
    ToggleButton playStop;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> playAttachment; // button value

    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintSelected(juce::Graphics& g);

    void loadBtnClicked();

    void thumbnailChanged();

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void envelopeSelected();

    void buttonClicked(Button* button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaGranaAudioProcessorEditor)
};
