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

    SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels, int notCeiledLength);
    SimpsonIntegrator(double* hilbertTransform, int samplingFrequency, int length, int numChannels, int notCeiledLength, float freqshift); //integrate shifted signal
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
    int notCeiledLength;
    int numChannels;

    float averageFrequency;
    float averageTime;

    double* hilbertSpectrum;
};

class Grain {

public:
    Grain(int grainDuration, int startPos);
    Grain(int grainDuration, int startPos, bool highreSolution);
    ~Grain();

    AudioBuffer<float>* processBuffer();
    AudioBuffer<float>* freqShift(float freqshift); //shifts every channel of freqShift [Hz] 
    void equalTemperament();
    inline float cubicinterp(float x, float y0, float y1, float y2, float y3);
    void changeEnvelope(EnvType type);

    float getCurrentSample(int channel);    //Return the current sample playing on the given channel

    //--------GETTERS AND SETTERS
    int getLength();
    int getCeiledLength(); //lowest power of 2 > grainlength, for fft and hilbert transforms
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
    bool isFinished();

private:
    int startPosition;      //in the loaded audio file
    int length;
    int currentPosition;    //current playing sample in the grain
    float fileSampleRate; 
    bool finished;

    float averageFrequency;
    float averageTime;
    float maxValue;

    bool highResolution;
    //int nextOnsetTime;


    FileLoader* fileLoader;
    AudioBuffer<float>* buffer;  //points to the whole audiobuffer, to be accessed with the index methods
    GrainEnvelope* envelope;
    int numChannels;   //number of channels
    int ceiledLength; //lowest power of 2 > grainlength, for fft and hilbert transforms
    double* hilbertTransform; //hilbert transform for each channel

    SimpsonIntegrator *integrator;

    void channelFreqShift(AudioBuffer<float>* buffer, float freqShift, int channel); //shifts a channel of freqshift [Hz]

    Array<AudioBuffer<float>*> freqShiftedGrains;
    Array<float> averageTimes;
    Array<float> averageFrequencies;
    Temperament temperament;

    int bufferHilbertIndex(int channel, int index);
    int bufferIndex(int channel, int index);

    void updateIndex();                     //Increment the current sample playing index. Set finish to true if the grain is finished
};


