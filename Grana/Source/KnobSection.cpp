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

    delete widthAttachment;
    delete typeAttachment;
 
}

void KnobSection::init(AudioProcessorValueTreeState& apvts, WaveformDrawable* waveform)
{
    //file position & section
    this->waveform = waveform;

    //FILE POSITION SLIDER
    filepos.setSliderStyle(Slider::LinearHorizontal);
    filepos.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filepos.onValueChange = [this] {this->waveform->repaint(); };
    labfilepos.setText("Filepos", dontSendNotification);
    labfilepos.setFont(Font(12.0f));
    labfilepos.setJustificationType(Justification(36));
    labfilepos.attachToComponent(&filepos, true);

    fileposAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts, "filepos", filepos));
    addAndMakeVisible(filepos);
    addAndMakeVisible(labfilepos);

    // SECTION SIZE SLIDER
    sectionsize.setSliderStyle(Slider::LinearHorizontal);
    sectionsize.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    sectionsize.onValueChange = [this] {this->waveform->repaint(); };
    labsectionsize.setText("Section", dontSendNotification);
    labsectionsize.setFont(Font(12.0f));
    labsectionsize.setJustificationType(Justification(36));
    labsectionsize.attachToComponent(&sectionsize, true);
    secsizeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(apvts, "Section Size", sectionsize));
    addAndMakeVisible(sectionsize);
    addAndMakeVisible(labsectionsize);

    //envelopes
    envelopeList.addItem("Gaussian", 1); //idx starting from 1!
    envelopeList.addItem("RaisedCosineBell", 2);
    envelopeList.addItem("Trapezoidal", 3);
    envelopeList.setSelectedId(1, dontSendNotification); // default value set to Gaussian
    envelopeList.onChange = [this] { envelopeSelected(); };
    typeAttachment = new ComboBoxParameterAttachment(*apvts.getParameter("envIndex"), envelopeList);
    addAndMakeVisible(envelopeList);
    addAndMakeVisible(envDraw);

    envShape.setSliderStyle(Slider::LinearHorizontal);
    envShape.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    widthAttachment = new SliderAttachment(apvts, "envWidth", envShape);
    envShape.onValueChange = [this] { widthChanged(); };
    envShapelab.setText("Envelope", dontSendNotification);
    envShapelab.setFont(Font(12.0f));
    envShapelab.setJustificationType(Justification(36));

    addAndMakeVisible(envShape);
    addAndMakeVisible(envShapelab);

    //grain parameters 
    for (int i = 0; i < NUM_CONTROLS; i++) {
        controls[i].setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        controls[i].setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);

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
    FlexBox level2;
    FlexBox knobBox;
    FlexBox envelopeBox;
    FlexBox positionBox;

    positionBox.flexDirection = FlexBox::Direction::column;
    positionBox.justifyContent = FlexBox::JustifyContent::center;

    positionBox.items.add(FlexItem(filepos).withFlex(1).withMargin(FlexItem::Margin(5, 35, 0, 50)));
    positionBox.items.add(FlexItem(sectionsize).withFlex(1).withMargin(FlexItem::Margin(0, 35, 5, 50)));

    envelopeBox.flexDirection = FlexBox::Direction::row;
    envelopeBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
    envelopeBox.items.add(FlexItem(envDraw).withFlex(2).withMargin(FlexItem::Margin(0,20,0,0)));
    FlexBox box2;
    box2.flexDirection = FlexBox::Direction::column;
    box2.justifyContent = FlexBox::JustifyContent::spaceAround;
    box2.items.add(FlexItem(envelopeList).withHeight(30).withMargin(FlexItem::Margin(4, 0, 40, 0)));
    box2.items.add(FlexItem(envShapelab).withFlex(0.5));
    box2.items.add(FlexItem(envShape).withFlex(2).withMargin(FlexItem::Margin(5, 0, 4, 0)));
    envelopeBox.items.add(FlexItem(box2).withFlex(2));


    knobBox.flexDirection = FlexBox::Direction::row;
    knobBox.justifyContent = FlexBox::JustifyContent::spaceBetween;

    for (auto& element : controls) {
        knobBox.items.add(FlexItem(element).withMargin(FlexItem::Margin(20, 0, 0, 0)).withFlex(1, 1));
    }

    level2.flexDirection = FlexBox::Direction::row;
    level2.items.add(FlexItem(envelopeBox).withFlex(2));
    level2.items.add(FlexItem(knobBox).withFlex(3));
    
    layout.flexDirection = FlexBox::Direction::column;
    layout.items.add(FlexItem(positionBox).withFlex(1).withMargin(FlexItem::Margin(0, 0, 10, 0)));
    layout.items.add(FlexItem(level2).withFlex(2));

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
    this->envDraw.setType(envelopeList.getSelectedId()); // set new type of envelope in the drawable
    this->envDraw.repaint(); // repaints the envelope
}

void KnobSection::widthChanged()
{
    this->envDraw.setWidth(envShape.getValue());
    this->envDraw.repaint();

}
