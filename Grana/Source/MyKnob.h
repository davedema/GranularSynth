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


    knobType getType() const;
    void setType(knobType type);

    int getRow() const;
    void setRow(int row);

private:
    knobType type;
    int row;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyKnob)


};
