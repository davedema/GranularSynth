/*
  ==============================================================================

    TrapezoidalEnvelope.h
    Created: 22 Sep 2020 5:27:29pm
    Author:  jacop

  ==============================================================================
*/

#define _USE_MATH_DEFINES
#include <cmath> 
#include "GrainEnvelope.h"
#include <iomanip> 
#include <vector>

#pragma once


class TrapezoidalEnvelope : public GrainEnvelope
{
public:
    TrapezoidalEnvelope(int sampleRate);
    TrapezoidalEnvelope(float duration, int sampleRate);
    TrapezoidalEnvelope(float duration, int sampleRate, float mainLobeWidth);
    float currentValue(float time) override;
    virtual float nextValue() override;

    void filterCreation();

    static TrapezoidalEnvelope* getInstance();

    ~TrapezoidalEnvelope();

private:
    float duration; //duration in seconds
    int sampleRate;
    float mainLobeWidth;
    std::vector<double> kernel;
};
