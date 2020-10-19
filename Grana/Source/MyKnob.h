/*
  ==============================================================================

    MyKnob.h
    Created: 31 May 2020 4:18:23pm
    Author:  daveg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
enum class knobType { amplitude, frequency, phase };
//==============================================================================
/*
*/
class MyKnob : public Slider
{
public:
    MyKnob() : Slider() {};
    MyKnob(Slider::SliderStyle style, TextEntryBoxPosition textBoxPos) : Slider(style, textBoxPos) {};
    ~MyKnob();

    knobType getType() const;
    void setType(knobType type);
    int getRow() const;
    void setRow(int row);
    void setAttachment(AudioProcessorValueTreeState& apvts, const String& id); //attach knob value to audio processor state

private:
    knobType type;
    // std::unique_ptr<Label> label; // text label for the knob
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> attachment; // knob value
    int row;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyKnob)


};
