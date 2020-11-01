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
#include "SimpsonIntegrator.h"
#include "./Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/math_const.h"

enum class EnvType {raisedCosineBell, gaussian, trapezoidal};

class Grain {

public:
    Grain(int grainDuration, int startPos, bool highreSolution, float freqShift, int envelopeType, float envelopeWidth, int hostRate, int direction);
    ~Grain();
    float getCurrentSample(int channel);    //Return the current sample playing on the given channel
    void updateIndex();                     //Increment the current sample playing index. Set finish to true if the grain is finished
    float getAverageFrequency();
    bool isFinished();
    AudioBuffer<float>* getBuffer();
    void setLag(int lag);
    void applyCrossFade(int crossfade, bool atStart);

private:
    int startPosition;      //in the loaded audio file
    int length;
    int currentPosition;    //current playing sample in the grain
    bool finished;
    float averageFrequency;
    int ceiledLength; //lowest power of 2 > grainlength, for fft and hilbert transforms
    double* hilbertTransform; //hilbert transform for each channel
    int lag;

    AudioBuffer<float>* buffer;  //points to the whole audiobuffer, to be accessed with the index methods
    SimpsonIntegrator *integrator;
    void channelFreqShift(float freqShift, int channel, int envType, float envWidth, int hostRate); //shifts a channel of freqshift [Hz]
    int bufferHilbertIndex(int channel, int index);
};


