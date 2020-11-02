/*
  ==============================================================================

    WaveformDrawable.h
    Created: 1 Nov 2020 4:55:47pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Model.h"
#include "FileLoader.h"

constexpr auto WAV_WIDTH = 480;
constexpr auto WAV_HEIGHT = 160;

class WaveformDrawable : public Component, public FileDragAndDropTarget, public ChangeListener
{
public:
    WaveformDrawable();
    ~WaveformDrawable();

    void init(AudioProcessorValueTreeState* apvts, Model* model);

    //component virtual methods
    void paint(Graphics&) override;
    void resized() override;

    //drag & drop vitual methods
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    //change listener on audio thumbnail
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
private:
    AudioProcessorValueTreeState* apvts;
    Model* model;
    FileLoader* loader;

    AudioThumbnail* thumbnail;

    void paintSelected(Graphics& g);
};