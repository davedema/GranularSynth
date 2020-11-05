/*
  ==============================================================================

    Model.h
    Created: 20 Oct 2020 10:56:03pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class Model : public AudioProcessorValueTreeState::Listener {
public:

    Model();

    void parameterChanged(const String& parameterID, float newValue) override;


    
    float getEnvWidth();
    int getSectionSize();
    float getDensity();
    int getEnvIndex();
    float getGrainSize();
    float getSpeedModule();
    bool getIsPlaying();
    void setIsPlaying(bool val);
    void setHasLoadedFile(bool hasDone, int fileLength);
    bool getHasLoadedFile();
    void setSampleRate(double sampleRate);
    int getSpeedDirection();
    bool getInit(); // if the granulator has been initialized
    void setInit(bool val);
    
    void setReadPosition(int readPosition);
    Array<Point<float>>* getxyPlane();     

    int getReadPosition();                                  //get next position of grain in file
    int getFilePos();                                       //get user starting position
    int getxyArrayPosition();                               //get current position in xyArray [0, xyPlane.size()]
    Point<float> getCurrentxyPosition();                    //get current position in xyplane [0, 1] X [0, 1] 
    float getCurrentFrequencyShift();                       //get current frequency shift

    int getCurrentTime();

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
    Array<Point<float>> xyPlane;
    bool init;
};