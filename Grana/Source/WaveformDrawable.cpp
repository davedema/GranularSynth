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
    juce::Rectangle<int> thumbnailBounds(0, 0, this->getWidth(), this->getHeight());

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
        //g.setColour(juce::Colours::darkgrey);
        // g.fillRect(thumbnailBounds);
       // g.setColour(juce::Colours::white);
       // g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
        g.setColour(juce::Colours::cadetblue);

        g.drawLine(Line<float>(0, this->getHeight() / 2, this->getWidth(), getHeight() / 2));

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
    int selectionWidth = floor(this->apvts->getRawParameterValue("Section Size")->load() * this->getWidth()); // tree state stores value in percentage!
    int filepos = floor(this->apvts->getRawParameterValue("filepos")->load() * this->getWidth());
    int rest = filepos + selectionWidth;
    Rectangle<int> selectionBounds;
    if (rest <= WAV_WIDTH)
        selectionBounds = Rectangle<int>(filepos, 0, selectionWidth, this->getHeight());
    else
        selectionBounds = Rectangle<int>(filepos, 0, (this->getWidth() - filepos), this->getHeight());

    g.setColour(Colours::darkred);
    g.setOpacity(0.4);
    g.fillRect(selectionBounds);
}
