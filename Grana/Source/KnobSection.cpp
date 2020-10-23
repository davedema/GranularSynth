/*
  ==============================================================================

    KnobSection.cpp
    Created: 26 May 2020 1:58:38pm
    Author:  daveg

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KnobSection.h"

//==============================================================================

KnobSection::KnobSection()
{
    xPos = 0;
    yPos = 0;
    height = 40;
    width = 40;
    checkDirection();
}



KnobSection::KnobSection(int x, int y, int w, int h, const std::vector<String>* ids, const std::vector<String>* titles, AudioProcessorValueTreeState* apvts) : xPos{ x }, yPos{ y }, width{ w }, height{ h } // nKnob is the length of ids
{
    checkDirection();
    addKnobs(ids, apvts, titles);
}

KnobSection::~KnobSection() {
    for (auto& knob : knobs) {
        delete knob;
    }

    for (auto& lab : labels) {
        delete lab;
    }

}


void KnobSection::setMyBounds() {
    this->setBounds(xPos, yPos, width, height);
}

void KnobSection::addKnobs(const std::vector<String>* ids, AudioProcessorValueTreeState* apvts, const std::vector<String>* titles)
{
    MyKnob* temp;
    Label* lab;


    for (size_t i = 0; i < ids->size(); ++i) {
        const String &id = ids->at(i);
        const String& title = titles->at(i);
        temp = new MyKnob(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);    //new MyKnob 
        temp->setTextBoxIsEditable(true);
        temp->setName(id);//set name 
        temp->setLookAndFeel(&KnobLAF);
        temp->setAttachment(*apvts, id); // attach knob value to audio processor tree state
        lab = new Label("title", title); //adds text label according to id
        // lab->attachToComponent(temp, false);
        //lab->setBounds(0, 0, width / 2, 30);
        // temp->addAndMakeVisible(lab);
        addAndMakeVisible(temp);
        knobs.push_back(temp);
        labels.push_back(lab);


    }

    arrange();


}


//TODO IMPLEMENT AUTOMATIC AND SCALABLE SETBOUND FOR KNOBS 
void KnobSection::arrange() { //arranges horyzontally or vertically according to width and heigth
    if (knobs.size() == 0) {
        return;
    }
    int i = 0;
    float individualWidth, individualHeight;

    switch (dir)
    {
    case direction::horizontal:
        individualHeight = height;
        individualWidth = width / knobs.size();
        for (auto& knob : knobs) {
            knob->setBounds(i * individualWidth, 0, individualWidth, individualHeight);
            i++;
        }
        break;

    case direction::vertical:
        individualHeight = height / knobs.size();
        individualWidth = width;
        for (auto& knob : knobs) {
            knob->setBounds(0, i * individualHeight, individualWidth, individualHeight);
            i++;
        }
        break;
    default:
        break;
    }


}


void KnobSection::checkDirection() {
    if (height > width) {
        dir = direction::vertical;
    }
    else
    {
        dir = direction::horizontal;
    }
}

void KnobSection::checkTypeAndSetRange(tSection type)
{
    switch (type)
    {
        //set knob range here
    case tSection::grain:

        break;

    default:
        break;
    }
}


void KnobSection::paint(Graphics& g)
{

}

void KnobSection::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void KnobSection::sliderDragStarted(Slider*)
{
}

void KnobSection::sliderDragEnded(Slider*)
{
}


void KnobSection::sliderValueChanged(Slider* slider)
{
    
}


void KnobSection::setMyOscillatorRange()
{
    /*
    int max = 100;
    int min = 10;
    float step = 0.01;

    knobs[0]->setRange(min / 100, max / 100, step);         // AMP
    knobs[0]->setValue(0.2);
    knobs[1]->setRange(min / 500, max / 10, step);          // FREQ RATIO
    knobs[1]->setSkewFactorFromMidPoint(max / 50);
    knobs[2]->setRange(0, 2 * double_Pi, step);           //PHASE    
    knobs[1]->setValue(1);                              // FREQ RATIO VALUES
    */
}

