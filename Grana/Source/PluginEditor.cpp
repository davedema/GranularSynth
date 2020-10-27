/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr auto GLOBAL_WIDTH = 800;
constexpr auto GLOBAL_HEIGHT = 500;

constexpr auto WAV_WIDTH = 480;
constexpr auto WAV_HEIGHT = 160;

//==============================================================================
LaGranaAudioProcessorEditor::LaGranaAudioProcessorEditor (LaGranaAudioProcessor& p): 
    AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(p.getValueTreeState())
{

    setSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);                                    
    loader = FileLoader::getInstance();                            //singleton
    formatManager = loader->getFormatManager();
    thumbnail = loader->getThumbnail();
    thumbnailCache = loader->getThumbnailCache();
    formatManager->registerBasicFormats();                         //register formats
    thumbnail->addChangeListener(this); // Listen to new file uploads


    //LOAD FILE BUTTON
    loadBtn = new juce::TextButton();
    addAndMakeVisible(loadBtn);
    loadBtn->setButtonText("Load...");
    loadBtn->onClick = [this] {loadBtnClicked();};
    loadBtn->setBounds(580, 40, 150, 50);
    //PLAY FILE BUTTON
    playStop.setBounds(580, 100, 150, 50);
    playStop.addListener(this);
    playStop.setButtonText("PLAY");
    playStop.setColour(TextButton::buttonColourId, Colours::green);
    playStop.setColour(TextButton::buttonOnColourId, Colours::red);
    addAndMakeVisible(playStop);
    playAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(*valueTreeState, "isPlaying", playStop));
    playStop.setToggleState(false, dontSendNotification);

    //FILE POSITION SLIDER
    filepos.setSliderStyle(Slider::LinearHorizontal);
    filepos.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filepos.onValueChange = [this] {hasChanged(); };
    labfilepos.setText("Filepos", dontSendNotification);
    labfilepos.setFont(Font(12.0f));
    labfilepos.setJustificationType(Justification(36));
    labfilepos.attachToComponent(&filepos, true);
    filepos.setBounds(40, 200, 480, 25);

    fileposAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(*valueTreeState, "filepos", filepos));
    addAndMakeVisible(filepos);
    addAndMakeVisible(labfilepos);

    // SECTION SIZE SLIDER
    sectionsize.setSliderStyle(Slider::LinearHorizontal);
    sectionsize.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    sectionsize.onValueChange = [this] {hasChanged(); };
    labsectionsize.setText("Section", dontSendNotification);
    labsectionsize.setFont(Font(12.0f));
    labsectionsize.setJustificationType(Justification(36));
    labsectionsize.attachToComponent(&sectionsize, true);
    sectionsize.setBounds(40, 240, 480, 25);
    secsizeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(*valueTreeState, "Section Size", sectionsize));
    addAndMakeVisible(sectionsize);
    addAndMakeVisible(labsectionsize);

    //GRAIN CONTROLS SECTION
    controlSection.init(*valueTreeState);
    addAndMakeVisible(controlSection);
   

}

LaGranaAudioProcessorEditor::~LaGranaAudioProcessorEditor()
{
    delete loadBtn;
}

//==============================================================================
void LaGranaAudioProcessorEditor::paint (juce::Graphics& g)
{

    juce::Rectangle<int> thumbnailBounds(40, 40, WAV_WIDTH, WAV_HEIGHT); 

    if (thumbnail->getNumChannels() == 0)
        paintIfNoFileLoaded(g, thumbnailBounds);
    else
    {
        paintIfFileLoaded(g, thumbnailBounds);
        paintSelected(g);
    }
}

void LaGranaAudioProcessorEditor::resized()
{
   
    int margin = 10;
    auto area = getLocalBounds().reduced(margin);

    // Add knob section below
    auto knobSection = area.removeFromLeft(controlSection.getWidth());
    controlSection.setBounds(knobSection.removeFromBottom(controlSection.getHeight()));
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

    g.setColour(juce::Colours::transparentBlack);
    g.fillRect(thumbnailBounds);

    g.setColour(juce::Colours::cadetblue);

    thumbnail->drawChannels(g,
        thumbnailBounds,
        0.0,                                    // start time
        thumbnail->getTotalLength(),             // end time
        1.0f);                                  // vertical zoom
    
}

void LaGranaAudioProcessorEditor::paintSelected(juce::Graphics& g)
{
    int selectionWidth = floor(this->valueTreeState->getRawParameterValue("Section Size")->load() * WAV_WIDTH / 100); // tree state stores value in percentage!
    int filepos = floor(this->valueTreeState->getRawParameterValue("filepos")->load() * WAV_WIDTH / 100); 
    int rest = filepos + selectionWidth;
    Rectangle<int> selectionBounds;
    if ( rest <= WAV_WIDTH ) 
        selectionBounds = Rectangle<int>(40 + filepos, 40, selectionWidth, WAV_HEIGHT);
    else
        selectionBounds = Rectangle<int>(40 + filepos, 40, (WAV_WIDTH - filepos), WAV_HEIGHT);

    g.setColour(Colours::darkred);
    g.setOpacity(0.4);
    g.fillRect(selectionBounds);
}


void LaGranaAudioProcessorEditor::loadBtnClicked() {

    juce::FileChooser chooser("Select a Wave file to play...",
        {},
        "*.wav");                                        // [7]

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto* reader = formatManager->createReaderFor(file);

        if (reader != nullptr) { 
            loader->loadWaveform(file); 
            audioProcessor.getModel()->setHasLoadedFile(true);
            audioProcessor.resetEnvelopes();
        }

        delete reader;
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
        audioProcessor.getModel()->setHasLoadedFile(true);
        audioProcessor.resetEnvelopes();
        //audioProcessor.granulate();
    }
}

void LaGranaAudioProcessorEditor::envelopeSelected()
{
    // create Envelope object
}

void LaGranaAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &playStop)
    {
        if (button->getToggleState()) { // if it is on 
            button->setButtonText("PLAY");
            button->setToggleState(false, dontSendNotification);
        }
        else
        {
            button->setButtonText("STOP");
            button->setToggleState(true, dontSendNotification);
            audioProcessor.play();
        }
    }
}

void LaGranaAudioProcessorEditor::hasChanged()
{
    repaint();
}

