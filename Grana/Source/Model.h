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
    int getRealPosition();
    void setRealPosition(int newPos);
    int getFilePos();                                       //get user starting position
    int getxyArrayPosition();                               //get current position in xyArray [0, xyPlane.size()]
    Point<float> getCurrentxyPosition();                    //get current position in xyplane [0, 1] X [0, 1] 
    float getCurrentFrequencyShift();                       //get current frequency shift

    int getCurrentTime();
    float getCurrentGain();
    int getSpread(); // in samples
    bool randomize();   //return true if the grain has to be randomized



private:
    Random r_spread;
    Random r_random;
    float filePos; // [0,1]
    float sectionSize; // [0,1]
    float envWidth;
    float density;
    int envIndex;
    float grainSize;
    float speedModule;
    bool isPlaying;
    bool hasLoadedFile;
    double sampleRate;
    int fileLength; // in samples
    int speedDirection;
    int readposition; // scanning position in the file
    int realPosition; // true position (considering random spread)
    Array<Point<float>> xyPlane;
    bool init;
    float currentGain;

    int spread;
    float random;
};