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
    setSize(480, 170);
}

KnobSection::~KnobSection() 
{
    for (int i = 0; i < NUM_CONTROLS; i++)
    {
        delete attachments[i];

    }
 
}

void KnobSection::init(AudioProcessorValueTreeState& apvts)
{
    for (int i = 0; i < NUM_CONTROLS; i++) {
        controls[i].setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        controls[i].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);

        labels[i].setText(controlNames[i], dontSendNotification);
        labels[i].setFont(Font(12.0f));
        labels[i].setJustificationType(Justification(36));
        labels[i].attachToComponent(&controls[i], false);

        attachments[i] = new SliderAttachment(apvts, controlNames[i], controls[i]);

        addAndMakeVisible(controls[i]);
        addAndMakeVisible(labels[i]);
    }
}

void KnobSection::paint(Graphics& g)
{

}

void KnobSection::resized()
{
    //Define the layout
    FlexBox layout;

    layout.flexDirection = FlexBox::Direction::row;
    layout.justifyContent = FlexBox::JustifyContent::center;

    for (auto& element : controls) {
        layout.items.add(FlexItem(element).withMargin(FlexItem::Margin(20, 0, 0, 0)).withFlex(1, 1));
    }

    layout.performLayout(getBounds().toFloat());
}

void KnobSection::sliderValueChanged(Slider* slider)
{
    
}

