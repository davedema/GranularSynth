/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class LaGranaAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::FileDragAndDropTarget
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

    juce::TextButton *loadBtn;
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioThumbnailCache* thumbnailCache;                  // [1]
    juce::AudioThumbnail* thumbnail;                            // [2]


    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);

    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);

    void loadBtnClicked();

    void loadWaveform(juce::File file, juce::AudioFormatReader* reader);

    void transportSourceChanged();

    void thumbnailChanged();

    // DRAG AND DROP 
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void fileLoader(const juce::String& gpath);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaGranaAudioProcessorEditor)
};
