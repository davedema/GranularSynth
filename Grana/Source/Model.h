/*
  ==============================================================================

    Model.h
    Created: 20 Oct 2020 10:56:03pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

enum class UserControls{filepos, randomPos, sectionSize, density, grainSize, speed, isPlaying, hasGranulatedSounds};

class Model : public AudioProcessorValueTreeState::Listener {
public:

    Model();

    virtual void parameterChanged(const String& parameterID, float newValue) override;


    float getFilePos();
    float getRandomPos();
    float getSectionSize();
    float getDensity();
    float getGrainSize();
    float getSpeed();
    bool getIsPlaying();
    void setHasGranulatedCloud(bool hasDone);
    bool getHasGranulatedCloud();

    bool detectChange(UserControls parameter);
    bool detectAnyChange();

private:

    float filePos;
    float randomPos;
    float sectionSize;
    float density;
    float grainSize;
    float speed;
    bool isPlaying;
    bool hasGranulatedCloud;

    Array<UserControls> changedParameters;

    float newFilePos;
    float newRandomPos;
    float newSectionSize;
    float newDensity;
    float newGrainSize;
    float newSpeed;



 
};