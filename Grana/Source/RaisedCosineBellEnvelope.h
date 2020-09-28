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
    RaisedCosineBellEnvelope(int sampleRate);
    RaisedCosineBellEnvelope(float duration, int sampleRate);
    RaisedCosineBellEnvelope(float duration, int sampleRate, float mainLobeWidth);
    float currentValue(float time) override;
    virtual float nextValue() override;

    static RaisedCosineBellEnvelope* getInstance();

    void filterCreation();

    ~RaisedCosineBellEnvelope();

private:
    float duration; //duration in seconds
    int sampleRate;
    float mainLobeWidth;
    std::vector<double> kernel;
};
