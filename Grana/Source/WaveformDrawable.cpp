/*
  ==============================================================================

    WaveformDrawable.cpp
    Created: 1 Nov 2020 4:55:47pm
    Author:  amere

  ==============================================================================
*/

#include "WaveformDrawable.h"

WaveformDrawable::WaveformDrawable()
{
    setSize(WAV_WIDTH, WAV_HEIGHT);

}
WaveformDrawable::~WaveformDrawable()
{
}

void WaveformDrawable::init(AudioProcessorValueTreeState* apvts, Model* model)
{
    this->apvts = apvts;
    this->model = model;
    this->loader = FileLoader::getInstance();
    this->thumbnail = loader->getThumbnail();
    thumbnail->addChangeListener(this); // Listen to new file uploads

}

void WaveformDrawable::paint(Graphics&g)
{
    juce::Rectangle<int> thumbnailBounds(40, 40, WAV_WIDTH, WAV_HEIGHT);

    if (this->model->getHasLoadedFile())
    {
        g.setColour(juce::Colours::transparentBlack);
        g.fillRect(thumbnailBounds);

        g.setColour(juce::Colours::cadetblue);

        thumbnail->drawChannels(g,
            thumbnailBounds,
            0.0,                                    // start time
            thumbnail->getTotalLength(),             // end time
            1.0f);                                  // vertical zoom

        paintSelected(g);
    }
    else {
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(thumbnailBounds);
        g.setColour(juce::Colours::white);
        g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
    }
}

void WaveformDrawable::resized()
{
}

bool WaveformDrawable::isInterestedInFileDrag(const StringArray& files)
{
    if (files[0].contains(".wav") || files[0].contains(".mp3")) return true;

    return false;
}

void WaveformDrawable::filesDropped(const StringArray& files, int x, int y)
{
    if (isInterestedInFileDrag(files)) {
        loader->loadFile(files[0]);
        this->model->setHasLoadedFile(true, loader->getAudioBuffer()->getNumSamples());
        this->model->setInit(false);
    }
}

void WaveformDrawable::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == thumbnail) repaint();
}

void WaveformDrawable::paintSelected(Graphics& g)
{
    int selectionWidth = floor(this->apvts->getRawParameterValue("Section Size")->load() * WAV_WIDTH); // tree state stores value in percentage!
    int filepos = floor(this->apvts->getRawParameterValue("filepos")->load() * WAV_WIDTH);
    int rest = filepos + selectionWidth;
    Rectangle<int> selectionBounds;
    if (rest <= WAV_WIDTH)
        selectionBounds = Rectangle<int>(40 + filepos, 40, selectionWidth, WAV_HEIGHT);
    else
        selectionBounds = Rectangle<int>(40 + filepos, 40, (WAV_WIDTH - filepos), WAV_HEIGHT);

    g.setColour(Colours::darkred);
    g.setOpacity(0.4);
    g.fillRect(selectionBounds);
}
