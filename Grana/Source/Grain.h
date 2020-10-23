/*
  ==============================================================================

    Grain.h
    Created: 17 Aug 2020 11:18:28am
    Author:  daveggggggggg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FileLoader.h"
#include "GrainEnvelope.h"
#include "GaussianEnvelope.h"
#include "RaisedCosineBellEnvelope.h"
#include "TrapezoidalEnvelope.h"

enum class EnvType {raisedCosineBell, gaussian, trapezoidal};

/*
Simpson integrator: integrates time and frequency using "Simpson rule".
Just create one and use the two public get methods to get average time and frequency
*/

class SimpsonIntegrator {
public:

    SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels);
    ~SimpsonIntegrator();
    float getAverageFrequency();
    float getAverageTime();

private:

    void computeAverageFrequency(double* hilbertTransform);
    void computeAverageTime(double* hilbertTransform);

    int samplingFrequency;
    int length;
    int numChannels;

    float averageFrequency;
    float averageTime;

    double* hilbertSpectrum;
};

class Grain {


private:

    /**long long onset;
    float lengthRecip;
    float amp;**/

    int length;
    int startPosition;
    int nextOnsetTime;
    float playbackRate; //siamo sicuri?

    float averageFrequency;
    float averageTime;


    FileLoader* fileLoader;
    AudioBuffer<float>* buffer;
    GrainEnvelope* envelope;
    int numChannels;   //number of channels
    int ceiledLength; //lowest power of 2 > grainlength, for fft and hilbert transforms
    double* hilbertTransform; //hilbert transform for each channel

    SimpsonIntegrator *integrator;
    

public:
    Grain(int length, int startPos);
    ~Grain();

    float maxValue;

    AudioBuffer<float>* processBuffer();


    inline float cubicinterp(float x, float y0, float y1, float y2, float y3);
    void changeEnvelope(EnvType type);
    void activate();
    void synthesize();



    //--------GETTERS AND SETTERS


    int getLength();
    float getSample(int channel, int index);
    int getNextOnsetTime();
    int getNumChannels();

};


