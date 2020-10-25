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

    delete envAttachment;
 
}

void KnobSection::init(AudioProcessorValueTreeState& apvts)
{
    envelopeList.addItem("Gaussian", 1); //idx starting from 1!
    envelopeList.addItem("RaisedCosineBell", 2);
    envelopeList.addItem("Trapezoidal", 3);
    envelopeList.onChange = [this] { envelopeSelected(); };
    addAndMakeVisible(envelopeList);

    envShape.setSliderStyle(Slider::LinearHorizontal);
    envShape.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    envAttachment = new SliderAttachment(apvts, "envShape", envShape);

    envShapelab.setText("Envelope", dontSendNotification);
    envShapelab.setFont(Font(12.0f));
    envShapelab.setJustificationType(Justification(36));
    envShapelab.attachToComponent(&envShape, false);

    addAndMakeVisible(envShape);
    addAndMakeVisible(envShapelab);


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
    FlexBox knobBox;
    FlexBox envelopeBox;

    envelopeBox.flexDirection = FlexBox::Direction::column;
    envelopeBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
    envelopeBox.items.add(FlexItem(envelopeList).withFlex(1).withMargin(FlexItem::Margin(0,0,20,0)));
    envelopeBox.items.add(FlexItem(envShape).withFlex(2));


    knobBox.flexDirection = FlexBox::Direction::row;
    knobBox.justifyContent = FlexBox::JustifyContent::center;

    for (auto& element : controls) {
        knobBox.items.add(FlexItem(element).withMargin(FlexItem::Margin(20, 0, 0, 0)).withFlex(1, 1));
    }

    layout.flexDirection = FlexBox::Direction::row;
    layout.items.add(FlexItem(envelopeBox).withFlex(1).withMargin(FlexItem::Margin(0, 20, 0, 0)));
    layout.items.add(FlexItem(knobBox).withFlex(2));
    layout.performLayout(getLocalBounds());
}

void KnobSection::sliderValueChanged(Slider* slider)
{
    if (&envShape == slider)
    {
        slider->getValue();
    }
}

void KnobSection::envelopeSelected()
{
}

void drawEnvelope(Array<float> values, int interpolation_factor, double env_width, int x, int y)
{
    Path envelopePath;
    Point<float> start((float)0, (float)(y));
    envelopePath.startNewSubPath(start);  // if this is the first point, start a new path..



}