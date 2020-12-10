/*
  ==============================================================================

    FileSection.cpp
    Created: 1 Nov 2020 3:51:30pm
    Author:  amere

  ==============================================================================
*/

#include "FileSection.h"

FileSection::FileSection()
{
    apvts = nullptr;
    model = nullptr;
    setSize(300, 100);
    loader = FileLoader::getInstance();
    //register audio formats
    formatManager = loader->getFormatManager();
    formatManager->registerBasicFormats();

}

FileSection::~FileSection()
{
}

void FileSection::init(AudioProcessorValueTreeState* apvts, Model* model)
{
    this->apvts = apvts;
    this->model = model;
    //LOAD FILE BUTTON
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("LOAD");
    loadButton.addListener(this);

    //PLAY FILE BUTTON
    playButton.setToggleState(false, dontSendNotification);
    playButton.addListener(this);
    playButton.setButtonText("PLAY");
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    playButton.setColour(TextButton::buttonOnColourId, Colours::red);
    addAndMakeVisible(playButton);
    playAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(*apvts, "isPlaying", playButton));

}

void FileSection::paint(Graphics&)
{
}

void FileSection::resized()
{
    FlexBox loadplay;

    loadplay.flexDirection = FlexBox::Direction::column;
    loadplay.justifyContent = FlexBox::JustifyContent::center;
    loadplay.alignContent = FlexBox::AlignContent::center;
    loadplay.items.add(FlexItem(loadButton).withHeight(60).withWidth(50).withMargin(4));
    loadplay.items.add(FlexItem(playButton).withHeight(60).withWidth(50).withMargin(4));

    loadplay.performLayout(getLocalBounds());
}

void FileSection::buttonClicked(Button* b)
{
    if (b == &playButton)
    {
        if (b->getToggleState()) { // if it is playing 
            b->setButtonText("PLAY");
            b->setToggleState(false, dontSendNotification);
            this->model->setIsPlaying(false);
            DBG(std::to_string(this->model->getIsPlaying()));

        }
        else
        {
            b->setButtonText("STOP");
            b->setToggleState(true, dontSendNotification);
            this->model->setIsPlaying(true);
            DBG(std::to_string(this->model->getIsPlaying()));
            this->model->setInit(false);
        }
    }

    else if (b == &loadButton)
    {
        FileChooser chooser("Select a Wave file to play...",
            {},
            "*.wav");
        this->model->setIsPlaying(false);
        this->playButton.setButtonText("PLAY");

        this->playButton.setToggleState(false, dontSendNotification);
        if (chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            auto* reader = this->formatManager->createReaderFor(file);

            if (reader != nullptr) {
                loader->loadWaveform(file);
                this->model->setHasLoadedFile(true, loader->getAudioBuffer()->getNumSamples());
            }

            delete reader;
        }
    }
}


