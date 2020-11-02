/*
  ==============================================================================

    FileSection.h
    Created: 1 Nov 2020 3:51:30pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FileLoader.h"
#include "Model.h"


typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; //type for slider attachment

class FileSection : public Component, public Button::Listener
{
public:
    FileSection();
    ~FileSection();

    void paint(Graphics&) override;
    void resized() override;
    void buttonClicked(Button* b) override;

    void init(AudioProcessorValueTreeState* apvts, Model* model);

    FileLoader* loader;

private:
    AudioFormatManager* formatManager; // to load formats like wav or mp3
    AudioProcessorValueTreeState* apvts;
    Model* model;
    TextButton loadButton;
    TextButton playButton;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> playAttachment; // button value
};