/*
  ==============================================================================

    FileSection.h
    Created: 21 Oct 2020 11:02:26pm
    Author:  amere

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FileSection : public Component, public Slider::Listener
{
public:
    FileSection();
    ~FileSection();
    void init(AudioProcessorValueTreeState& apvts);
    void paint(Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;

private:



};