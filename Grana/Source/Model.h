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


    int getFilePos();
    float getEnvWidth();
    int getSectionSize();
    float getDensity();
    int getEnvIndex();
    float getGrainSize();
    float getSpeedModule();
    bool getIsPlaying();
    void setHasLoadedFile(bool hasDone, int fileLength);
    bool getHasLoadedFile();
    void setSampleRate(double sampleRate);
    int getSpeedDirection();
    int getReadPosition();
    void setReadPosition(int readPosition);

private:

    float filePos;
    float sectionSize;
    float envWidth;
    float density;
    int envIndex;
    float grainSize;
    float speedModule;
    bool isPlaying;
    bool hasLoadedFile;
    double sampleRate;
    int fileLength;
    int speedDirection;
    int readposition;
};