/*
  ==============================================================================

    RaisedCosineBellEnvelope.h
    Created: 22 Sep 2020 5:27:14pm
    Author:  jacop

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES
#include <cmath> 
#include "GrainEnvelope.h"
#include <iomanip> 
#include <vector>



#pragma once


class RaisedCosineBellEnvelope : public GrainEnvelope
{
public:
    float currentValue(float time) override;
    virtual float nextValue() override;

    static void reset();
    static void reset(float duration, int sampleRate, float mainLobeWidth);

    static RaisedCosineBellEnvelope* getInstance();
    static RaisedCosineBellEnvelope* setSampleRate(int sampleRate);
    static RaisedCosineBellEnvelope* setMainLobeWidth(int mainLobeWidth);
    static RaisedCosineBellEnvelope* setDuration(int duration);

private:
    RaisedCosineBellEnvelope();
    RaisedCosineBellEnvelope(int sampleRate);
    RaisedCosineBellEnvelope(float duration, int sampleRate);
    RaisedCosineBellEnvelope(float duration, int sampleRate, float mainLobeWidth);

    ~RaisedCosineBellEnvelope();

    void filterCreation();

    float duration; //duration in seconds
    int sampleRate;
    float mainLobeWidth;
    std::vector<double> kernel;
    static RaisedCosineBellEnvelope* instance;
};
