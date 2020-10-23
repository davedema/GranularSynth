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
enum class Temperament{equalTemperament};

/*
Simpson integrator: integrates time and frequency using "Simpson rule".
Just create one and use the two public get methods to get average time and frequency
*/

class SimpsonIntegrator {
public:

    SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels);
    SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels, float freqshift); //integrate shifted signal
    ~SimpsonIntegrator();
    float getAverageFrequency();
    float getAverageTime();

private:

    void computeAverageFrequency(double* hilbertTransform);
    void computeAverageTime(double* hilbertTransform);
    void computeAverageFrequency(double* hilbertTransform, float freqShift);
    void computeAverageTime(double* hilbertTransform, float freqShift);

    int samplingFrequency;
    int length;
    int numChannels;

    float averageFrequency;
    float averageTime;

    double* hilbertSpectrum;
};

class Grain {


private:

    int length;
    int startPosition;
    int nextOnsetTime;
    float sampleRate; 

    float averageFrequency;
    float averageTime;
    float maxValue;

    bool highResolution;



    FileLoader* fileLoader;
    AudioBuffer<float>* buffer;
    GrainEnvelope* envelope;
    int numChannels;   //number of channels
    int ceiledLength; //lowest power of 2 > grainlength, for fft and hilbert transforms
    double* hilbertTransform; //hilbert transform for each channel

    SimpsonIntegrator *integrator;

    void channelFreqShift(AudioBuffer<float>* buffer, float freqShift, int channel); //shifts a channel of freqshift [Hz]
    AudioBuffer<float>* freqShift(float freqshift); //shifts every channel of freqShift [Hz] 

    Array<AudioBuffer<float>*> freqShiftedGrains;
    Array<float> averageTimes;
    Array<float> averageFrequencies;
    Temperament temperament;

public:
    Grain(int length, int startPos);
    Grain(int length, int startPos, bool highreSolution);
    ~Grain();

    AudioBuffer<float>* processBuffer();

    void equalTemperament();


    inline float cubicinterp(float x, float y0, float y1, float y2, float y3);
    void changeEnvelope(EnvType type);
    void activate();
    void synthesize();

    



    //--------GETTERS AND SETTERS


    int getLength();
    int getCeiledLength(); //lowest power of 2 > grainlength, for fft and hilbert transforms
    float getSample(int channel, int index);
    int getNextOnsetTime();
    int getNumChannels();
    float getMaxValue();
    int getStartPosition();
    float getAverageFrequency();
    float getAverageTime();
    GrainEnvelope* getEnvelope();
    AudioBuffer<float>* getBuffer();  
    double* getHilbertTransform(); //hilbert transform for each channel --> 2 * channel * ceiledlength samples 
    Array<AudioBuffer<float>*> getFreqShiftedGrains();
    

};


