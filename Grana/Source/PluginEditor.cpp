/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaGranaAudioProcessorEditor::LaGranaAudioProcessorEditor (LaGranaAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p){

    loader = FileLoader::getInstance();

    formatManager = loader->getFormatManager();
   // transportSource = loader->getTransportSource();
    thumbnail = loader->getThumbnail();
    thumbnailCache = loader->getThumbnailCache();


    formatManager->registerBasicFormats();
    loadBtn = new juce::TextButton();
    addAndMakeVisible(loadBtn);
    loadBtn->setButtonText("Open...");
    loadBtn->onClick = [this] {loadBtnClicked();};
    loadBtn->setBounds(10, 10, 50, 25);


    thumbnail->addChangeListener(this);


    setSize (400, 300);
}

LaGranaAudioProcessorEditor::~LaGranaAudioProcessorEditor()
{
    delete loadBtn;
    delete thumbnailCache, thumbnail;
}

//==============================================================================
void LaGranaAudioProcessorEditor::paint (juce::Graphics& g)
{

    juce::Rectangle<int> thumbnailBounds(40, 100, getWidth() - 20, getHeight() - 120);

    if (thumbnail->getNumChannels() == 0)
        paintIfNoFileLoaded(g, thumbnailBounds);
    else
        paintIfFileLoaded(g, thumbnailBounds);
}

void LaGranaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void LaGranaAudioProcessorEditor::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
    
}

void LaGranaAudioProcessorEditor::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{

    
    g.setColour(juce::Colours::white);
    g.fillRect(thumbnailBounds);

    g.setColour(juce::Colours::red);                               // [8]

    thumbnail->drawChannels(g,                                      // [9]
        thumbnailBounds,
        0.0,                                    // start time
        thumbnail->getTotalLength(),             // end time
        1.0f);                                  // vertical zoom
    
}

void LaGranaAudioProcessorEditor::loadBtnClicked() {

    juce::FileChooser chooser("Select a Wave file to play...",
        {},
        "*.wav");                                        // [7]

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto* reader = formatManager->createReaderFor(file);

        if (reader != nullptr) { loader->loadWaveform(file); }
    }
}


void LaGranaAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    //if (source == &transportSource) transportSourceChanged();
    if (source == thumbnail)       thumbnailChanged();
}


void LaGranaAudioProcessorEditor::thumbnailChanged()
{
    repaint();
}


// DRAG AND DROP
bool LaGranaAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    if (files[0].contains(".wav") || files[0].contains(".mp3")) return true;

    return false;
}

void LaGranaAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (isInterestedInFileDrag(files)) {
        loader->loadFile(files[0]);
    }
}