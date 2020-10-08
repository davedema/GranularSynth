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
    AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(p.getValueTreeState()){

    setSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);                                    
    loader = FileLoader::getInstance();                            //singleton
    formatManager = loader->getFormatManager();
   // transportSource = loader->getTransportSource();
    thumbnail = loader->getThumbnail();
    thumbnailCache = loader->getThumbnailCache();


    formatManager->registerBasicFormats();                         //register formats
    loadBtn = new juce::TextButton();
    addAndMakeVisible(loadBtn);
    loadBtn->setButtonText("Open...");
    loadBtn->onClick = [this] {loadBtnClicked();};
    loadBtn->setBounds(10, 10, 50, 25);

    thumbnail->addChangeListener(this);

    //FILE MANAGEMENT SECTION
    const std::vector<String> fileids = { "filepos", "randompos" },    //knob ids
        * fileids_ptr = &fileids;                                        //pointer to knob ids
    const std::vector<String> filetitles = { "File position", "Random" }, * filetitles_ptr = &filetitles;

    fileSection = new KnobSection(
        WAV_WIDTH + 100,
        WAV_HEIGHT / 2 - 10,
        100, 70,
        fileids_ptr,
        filetitles_ptr,
        p.getValueTreeState()
        );
    fileSection->setMyBounds();
    addAndMakeVisible(fileSection);
    
    // GRAIN SECTION
    const std::vector<String> grainids = {"grain_durations", "grain_density"},    //knob ids
        *point = &grainids;                                        //pointer to knob ids
    const std::vector<String> titles = { "Duration", "Density" }, *titlesp = &titles;
    grainSection = new KnobSection(                                //new knobsection
        40, 
        getHeight() / 2,
        getWidth() / 3 - 80, getHeight() / 4 - 50,
        point,
        titlesp,
        p.getValueTreeState()
    );
    grainSection->setMyBounds();                                    //set bounds
    addAndMakeVisible(grainSection);

    //ENVELOPE SECTION
    envelopeList = new ComboBox();
    envelopeList->setBounds(getWidth() / 3 , getHeight() / 2, 100,30);
    addAndMakeVisible(envelopeList);
    envelopeList->addItem("Gaussian", 1);
    envelopeList->addItem("RaisedCosineBell", 2);
    envelopeList->addItem("Trapezoidal", 3);
    envelopeList->onChange = [this] { envelopeSelected(); };

}

LaGranaAudioProcessorEditor::~LaGranaAudioProcessorEditor()
{
    delete loadBtn;
    delete grainSection;
    delete envelopeList;
    delete fileSection;
}

//==============================================================================
void LaGranaAudioProcessorEditor::paint (juce::Graphics& g)
{

    juce::Rectangle<int> thumbnailBounds(40, 40, WAV_WIDTH, WAV_HEIGHT); 

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

        if (reader != nullptr) { 
            loader->loadWaveform(file); 
            audioProcessor.resetEnvelopes();
            audioProcessor.granulate();
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
        audioProcessor.resetEnvelopes();
        audioProcessor.granulate();
    }
}

void LaGranaAudioProcessorEditor::envelopeSelected()
{
    // create Envelope object
}
