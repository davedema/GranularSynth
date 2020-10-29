/*
  ==============================================================================

    Model.h
    Created: 20 Oct 2020 10:56:03pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

enum class UserControls{filepos, sectionSize, envIndex, envWidth, density, grainSize, speed, isPlaying, hasGranulatedSounds};

class Model : public AudioProcessorValueTreeState::Listener {
public:

    Model();

    void parameterChanged(const String& parameterID, float newValue) override;


    float getFilePos();
    float getEnvWidth();
    float getSectionSize();
    float getDensity();
    int getEnvIndex();
    float getGrainSize();
    float getSpeed();
    bool getIsPlaying();
    void setHasLoadedFile(bool hasDone);
    bool getHasLoadedFile();
    void setSampleRate(double sampleRate);

private:

    float filePos;
    float sectionSize;
    float envWidth;
    float density;
    int envIndex;
    float grainSize;
    float speed;
    bool isPlaying;
    bool hasLoadedFile;
    double sampleRate;
};