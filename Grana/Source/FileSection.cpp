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
    loadButton.setButtonText("Load...");
    loadButton.addListener(this);

    //PLAY FILE BUTTON
    playButton.addListener(this);
    playButton.setButtonText("PLAY");
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    playButton.setColour(TextButton::buttonOnColourId, Colours::red);
    addAndMakeVisible(playButton);
    playAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(*apvts, "isPlaying", playButton));
    playButton.setToggleState(false, dontSendNotification);
}

void FileSection::paint(Graphics&)
{
}

void FileSection::resized()
{
    FlexBox container;
    container.flexDirection = FlexBox::Direction::row;
    container.items.add(FlexItem(loadButton).withFlex(1));
    container.items.add(FlexItem(playButton).withFlex(1));

    container.performLayout(getLocalBounds());
}

void FileSection::buttonClicked(Button* b)
{
    if (b == &playButton)
    {
        if (b->getToggleState()) { // if it is on 
            b->setButtonText("PLAY");
            b->setToggleState(false, dontSendNotification);
        }
        else
        {
            b->setButtonText("STOP");
            b->setToggleState(true, dontSendNotification);
            this->model->setInit(false);
        }
    }

    else if (b == &loadButton)
    {
        FileChooser chooser("Select a Wave file to play...",
            {},
            "*.wav");

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


