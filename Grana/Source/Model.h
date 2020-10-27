/*
  ==============================================================================

    Model.h
    Created: 20 Oct 2020 10:56:03pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

enum class UserControls{filepos, sectionSize, envIndex, envAmt, density, grainSize, speed, isPlaying, hasGranulatedSounds};

class Model : public AudioProcessorValueTreeState::Listener {
public:

    Model();

    void parameterChanged(const String& parameterID, float newValue) override;


    float getFilePos();
    float getenvAmt();
    float getSectionSize();
    float getDensity();
    int getEnvIndex();
    float getGrainSize();
    float getSpeed();
    bool getIsPlaying();
    void setHasLoadedFile(bool hasDone);
    bool getHasLoadedFile();

    bool detectChange(UserControls parameter);
    bool detectAnyChange();

private:

    float filePos;
    float sectionSize;
    float envAmt;
    float density;
    int envIndex;
    float grainSize;
    float speed;
    bool isPlaying;
    bool hasLoadedFile;

    Array<UserControls> changedParameters;

    float newenvAmt;


 
};