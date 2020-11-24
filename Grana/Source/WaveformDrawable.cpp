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
    startTimerHz(60);


}
WaveformDrawable::~WaveformDrawable()
{
    stopTimer();
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
        g.setColour(Colour(ColourPalette::background).withMultipliedAlpha(0.3));
        g.fillRect(thumbnailBounds);
        g.setColour(Colour(ColourPalette::dark_component));

        thumbnail->drawChannels(g,
            thumbnailBounds,
            0.0,                                    // start time
            thumbnail->getTotalLength(),             // end time
            1.0f);                                  // vertical zoom

        paintSelected(g);
    }
    else {
        g.setColour(Colour(ColourPalette::dark_component));

        g.drawLine(Line<float>(0, this->getHeight() / 2, this->getWidth(), getHeight() / 2));
    }

        if (this->model->getIsPlaying()) 
            this->paintPlayBar(g);

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

void WaveformDrawable::timerCallback()
{
    if (this->model->getIsPlaying() && this->model->getHasLoadedFile())
        repaint();

}

void WaveformDrawable::paintSelected(Graphics& g)
{
    int selectionWidth = floor(this->apvts->getRawParameterValue("Section Size")->load() * this->getWidth()); // tree state stores value in percentage!
    int filepos = floor(this->apvts->getRawParameterValue("filepos")->load() * this->getWidth());
    int rest = filepos + selectionWidth;
    Rectangle<int> selectionBounds;
    if (rest <= this->getWidth())
        selectionBounds = Rectangle<int>(filepos, 0, selectionWidth, this->getHeight());
    else
        selectionBounds = Rectangle<int>(filepos, 0, (this->getWidth() - filepos), this->getHeight());

    g.setColour(Colour(ColourPalette::numbers));
    g.setOpacity(0.4);
    g.fillRect(selectionBounds);
}

void WaveformDrawable::paintPlayBar(Graphics& g)
{
    repaint();
    g.setColour(Colours::yellow);
    int current_pos = round(this->model->getCurrentTime() * this->getWidth() / loader->getAudioBuffer()->getNumSamples());

    g.drawVerticalLine(current_pos, 0, this->getHeight());
    paintGrains(g, current_pos);

    g.setOpacity(0.8); 
    g.setColour(Colours::red);
    current_pos = round(this->model->getRealPosition() * this->getWidth() / loader->getAudioBuffer()->getNumSamples());
    g.drawVerticalLine(current_pos, this->getHeight() / 3, this->getHeight() * 2 / 3);


}

void WaveformDrawable::paintGrains(Graphics& g, int current_pos)
{
    int n_grains = ceil(jmap(this->model->getDensity() / GRAIN_DENSITY_MAX, 0.0f, 20.0f));
    int mid = this->getHeight() / 2;
    g.setColour(Colours::white);
    
    auto r = Random();
    for (int i = 0; i < n_grains; i++)
    {
        Point<float> grain ( (float) current_pos + r.nextInt(Range<int>(-5,5)),
                              (float)i / n_grains * mid + r.nextInt(Range<int>(-mid, mid)));

        g.fillEllipse(grain.x, grain.y, 8.0f, 8.0f);
                                
    }
}

