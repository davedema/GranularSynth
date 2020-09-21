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

KnobSection::KnobSection(int x, int y, int w, int h) : xPos{ x }, yPos{ y }, width{ w }, height{ h }{
    checkDirection();
}

KnobSection::KnobSection(int x, int y, int w, int h, int nKnob) : xPos{ x }, yPos{ y }, width{ w }, height{ h }{
    checkDirection();;
    addKnobs(nKnob);
}

KnobSection::KnobSection(int x, int y, int w, int h, int nKnob, tSection type) : xPos{ x }, yPos{ y }, width{ w }, height{ h }
{
    checkDirection();
    //addKnobs(nKnob);
    addKnobs(nKnob, -1);
}

KnobSection::KnobSection(int x, int y, int w, int h, int nKnob, tSection type, int row) : xPos{ x }, yPos{ y }, width{ w }, height{ h }
{
    checkDirection();
    addKnobs(nKnob, row);
}

KnobSection::KnobSection(int x, int y, int w, int h, int nKnob, tSection type, int row, Slider::Listener* listener) : xPos{ x }, yPos{ y }, width{ w }, height{ h }
{
    checkDirection();
    addKnobs(nKnob, row, listener);
}

KnobSection::KnobSection(int x, int y, int w, int h, const std::vector<String>* ids, AudioProcessorValueTreeState* apvts) : xPos{ x }, yPos{ y }, width{ w }, height{ h } // nKnob is the length of ids
{
    checkDirection();
    addKnobs(ids, apvts);
}

KnobSection::~KnobSection() {
    for (auto& knob : knobs) {
        delete knob;
    }
}


void KnobSection::setMyBounds() {
    this->setBounds(xPos, yPos, width, height);
}


void KnobSection::addKnobs(int nKnob)
{
    addKnobs(nKnob, -1, this);
}

void KnobSection::addKnobs(int nKnob, int row)
{
    addKnobs(nKnob, row, this);
}


void KnobSection::addKnobs(int nKnob, int row, Slider::Listener* listener)
{
 
}

void KnobSection::addKnobs(const std::vector<String>* ids, AudioProcessorValueTreeState* apvts)
{
    MyKnob* temp;
    String knobName = "general";


    for (size_t i = 0; i < ids->size(); ++i) {
        temp = new MyKnob(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
        temp->setTextBoxIsEditable(true);
        temp->setRange(0, 10, 0.1);
        std::string stdName = knobName.toStdString();
        temp->setName(knobName); //set name to send to socket
        temp->setLookAndFeel(&KnobLAF);
        //TO DO connect to audio processor
        const String &id = ids->at(i);
        temp->setAttachment(*apvts, id);
        addAndMakeVisible(temp); // makes visible each knob
        knobs.push_back(temp);
    }

    arrange();


}


//TODO IMPLEMENT AUTOMATIC AND SCALABLE SETBOUND FOR KNOBS 
void KnobSection::arrange() {
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

