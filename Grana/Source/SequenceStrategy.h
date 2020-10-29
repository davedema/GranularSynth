/*
  ==============================================================================

    SequenceStrategy.h
    Created: 15 Sep 2020 2:20:49pm
    Author:  jacop

  ==============================================================================

  Provides an interface allowing the Scheduler to determine when the next grain should occur andwhat its duration should be

*/

#pragma once

#include <stdlib.h>
#include <random>
#include "Grain.h"
#include "Model.h"

class SequenceStrategy {

public:
    SequenceStrategy();
    int nextInterOnset(int userLength);
    int nextInterOnset(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer);
    void setQuasiSyncRange(float quasiSyncRange);
    void setSampleRate(double sampleRate);

    void setModel(Model* model);
    int getNextOnset();

private:
    float quasiSyncRange;
    double sampleRate;

    Model* model;

    Array<float>* computeCrossCorrelation(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength);
    void crossFade(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int interOnset, int grainLength);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 engine; //Standard mersenne_twister_engine seeded with rd() and feeded to distribution
    std::uniform_real_distribution<float> distribution; //uniform distribution
};