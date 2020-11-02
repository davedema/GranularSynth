/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

constexpr auto GLOBAL_WIDTH = 800;
constexpr auto GLOBAL_HEIGHT = 600;

//==============================================================================
LaGranaAudioProcessorEditor::LaGranaAudioProcessorEditor (LaGranaAudioProcessor& p): 
    AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(p.getValueTreeState())
{
    p.setFeatureDrawers(&this->spectrum); 
    xyPlane.setModel(p.getModel());


    setSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);                         
    
    //draw waveform
    //upload and play file
    fileSection.init(valueTreeState, p.getModel());
    addAndMakeVisible(fileSection);

    waveform.init(valueTreeState, p.getModel());
    addAndMakeVisible(waveform);


    //GRAIN CONTROLS SECTION + POSITION CONTROL
    controlSection.init(*valueTreeState, &waveform);
    addAndMakeVisible(controlSection);
   
    //OUT SPECTRUM
    spectrum.setBounds(520, this->getHeight() / 2 - 100, 280, 150);
    addAndMakeVisible(spectrum);

    //XY plane
    addAndMakeVisible(xyPlane);
    xyPlane.setBounds(520, this->getHeight() / 2 + 100 , 200, 150);
}

LaGranaAudioProcessorEditor::~LaGranaAudioProcessorEditor()
{
}

//==============================================================================
void LaGranaAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void LaGranaAudioProcessorEditor::resized()
{
    FlexBox container; //main container

    FlexBox fileContainer; //waveform + open/play controls
    FlexBox secondLevel; // row below containing grain and features
    FlexBox grainContainer; // grain section with controls on grain size, envelopes etc...
    FlexBox featuresContainer; //output results

    fileContainer.flexDirection = FlexBox::Direction::row;
    fileContainer.items.add(FlexItem(waveform).withFlex(3));
    fileContainer.items.add(FlexItem(fileSection).withFlex(1));

    secondLevel.flexDirection = FlexBox::Direction::row;
    secondLevel.items.add(FlexItem(grainContainer).withFlex(2));
    secondLevel.items.add(FlexItem(featuresContainer).withFlex(1));

    grainContainer.flexDirection = FlexBox::Direction::column;
    grainContainer.justifyContent = FlexBox::JustifyContent::spaceAround;
    grainContainer.items.add(FlexItem(controlSection).withFlex(2));


    container.flexDirection = FlexBox::Direction::column;
    container.items.add(FlexItem(fileContainer).withFlex(1));
    container.items.add(FlexItem(secondLevel).withFlex(2));

    container.performLayout(getLocalBounds());



}
