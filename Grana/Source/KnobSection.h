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

//==============================================================================
/*
*/

enum class direction { horizontal, vertical };
enum class tSection { general };

class KnobSection : public Component, public Slider::Listener
{
public:
    KnobSection();
    KnobSection(int x, int y, int w, int h);
    KnobSection(int x, int y, int w, int h, int nKnob);
    KnobSection(int x, int y, int w, int h, int nKnob, tSection type);
    KnobSection(int x, int y, int w, int h, int nKnob, tSection type, int row);
    KnobSection(int x, int y, int w, int h, int nKnob, tSection type, int row, Slider::Listener* listener);

    ~KnobSection();


    void paint(Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
    void sliderDragStarted(Slider*) override;
    void sliderDragEnded(Slider*) override;


    void setMyBounds();
    void addKnobs(int nKnob);
    void addKnobs(int nKnob, int row);
    void addKnobs(int nKnob, int row, Slider::Listener* listener);

    void setMyOscillatorRange();

private:
    std::vector<MyKnob*> knobs;
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
