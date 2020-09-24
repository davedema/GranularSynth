/*
  ==============================================================================

    KnobSection.h
    Created: 26 May 2020 1:58:38pm
    Author:  daveg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "KnobLookAndFeel.h"
#include "MyKnob.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; //type for slider attachment

//==============================================================================
/*
*/

enum class direction { horizontal, vertical };
enum class tSection { grain };

class KnobSection : public Component, public Slider::Listener
{
public:
    KnobSection();

    KnobSection(int x, int y, int w, int h, const std::vector<String>* ids, AudioProcessorValueTreeState* apvts);

    ~KnobSection();


    void paint(Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;

    void setMyBounds();
    void addKnobs(const std::vector<String>* ids, AudioProcessorValueTreeState* apvts);

    void setMyOscillatorRange();

private:
    std::vector<MyKnob*> knobs;
    std::vector<Label*> labels;

    int xPos;
    int yPos;
    int width;
    int height;
    direction dir;

    void arrange();
    void checkDirection();
    void checkTypeAndSetRange(tSection type);

    KnobLookAndFeel KnobLAF;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobSection)
};
