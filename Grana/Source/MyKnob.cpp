/*
  ==============================================================================

    MyKnob.cpp
    Created: 31 May 2020 4:18:23pm
    Author:  daveg

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MyKnob.h"

//==============================================================================



MyKnob::~MyKnob()
{
    attachment.reset(nullptr);
     
}

knobType MyKnob::getType() const
{
    return type;
}

void MyKnob::setType(knobType type)
{
    this->type = type;
}


int MyKnob::getRow() const
{
    return row;
}

void MyKnob::setRow(int row)
{
    this->row = row;
}


void MyKnob::setAttachment(AudioProcessorValueTreeState& apvts, const String& id)
{
    // knobValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, id, this);
    attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts, id, *this));
}