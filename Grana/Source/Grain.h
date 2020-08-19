/*
  ==============================================================================

    Grain.h
    Created: 17 Aug 2020 11:18:28am
    Author:  daveg

  ==============================================================================
*/


#include <JuceHeader.h>
#include "GrainWindow.h"


class Grain : public juce::AudioBuffer<int>{


private:
    WindowType winType;


public:

    Grain(WindowType type) : AudioBuffer() { winType = type; };
    WindowType getWinType() const;
    void setWinType(WindowType winType);   
};
