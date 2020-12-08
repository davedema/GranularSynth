/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

constexpr auto GLOBAL_WIDTH = 850;
constexpr auto GLOBAL_HEIGHT = 700;

//==============================================================================
LaGranaAudioProcessorEditor::LaGranaAudioProcessorEditor (LaGranaAudioProcessor& p): 
    AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(p.getValueTreeState())
{    
    this->setLookAndFeel(&CLAF);
    p.setFeatureDrawers(&this->spectrum); 
    xyPlane.setModel(p.getModel());

    setSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);  
    
    //draw waveform
    //upload and play file
    fileSection.init(valueTreeState, p.getModel());
    addAndMakeVisible(fileSection);

    waveform.init(valueTreeState, p.getModel());
    addAndMakeVisible(waveform);

    //master gain slider
    masterGain.setSliderStyle(Slider::LinearVertical);
    masterGain.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    masterGainAttachment.reset(new SliderAttachment(*valueTreeState, "masterGain", masterGain));
    masterGainlab.setText("Master gain", dontSendNotification);
    masterGainlab.setFont(Font(12.0f));
    masterGainlab.setJustificationType(Justification(36));
    masterGainlab.attachToComponent(&masterGain, false);
    addAndMakeVisible(masterGain);
    addAndMakeVisible(masterGainlab);


    //GRAIN CONTROLS SECTION + POSITION CONTROL
    controlSection.init(*valueTreeState, &waveform);
    addAndMakeVisible(controlSection);
   
    //OUT SPECTRUM
    addAndMakeVisible(spectrum);

    //XY plane
    addAndMakeVisible(xyPlane);
}

LaGranaAudioProcessorEditor::~LaGranaAudioProcessorEditor()
{
    this->setLookAndFeel(nullptr);

}

//==============================================================================
void LaGranaAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colour(ColourPalette::background).darker(0.8));

}

void LaGranaAudioProcessorEditor::resized()
{
    FlexBox container; //main container

    FlexBox firstRow; // open/play + waveform + master gain
    FlexBox secondRow;  // grain controls
    FlexBox thirdRow;  // output features

    firstRow.flexDirection = FlexBox::Direction::row;
    firstRow.items.add(FlexItem(fileSection).withWidth(60).withMargin(10));
    firstRow.items.add(FlexItem(waveform).withFlex(1));
    firstRow.items.add(FlexItem(masterGain).withWidth(60));

    secondRow.flexDirection = FlexBox::Direction::row;
    secondRow.items.add(FlexItem(controlSection).withFlex(1));

    thirdRow.flexDirection = FlexBox::Direction::row;
    thirdRow.justifyContent = FlexBox::JustifyContent::spaceAround;
    thirdRow.items.add(FlexItem(xyPlane).withFlex(1).withMargin(20));
    thirdRow.items.add(FlexItem(spectrum).withFlex(1).withMargin(20));

    container.flexDirection = FlexBox::Direction::column;
    container.justifyContent = FlexBox::JustifyContent::spaceAround;
    container.items.add(FlexItem(firstRow).withFlex(1.5).withMargin(FlexItem::Margin(20,5,5,5)));
    container.items.add(FlexItem(secondRow).withFlex(2).withMargin(FlexItem::Margin(0, 5, 10, 5)));
    container.items.add(FlexItem(thirdRow).withFlex(2).withMargin(5));


    container.performLayout(getLocalBounds());



}
