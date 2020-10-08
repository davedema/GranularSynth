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




#pragma once


class RaisedCosineBellEnvelope : public GrainEnvelope
{
public:

    static void reset();
    static void reset(int duration, int sampleRate, float mainLobeWidth);

    static RaisedCosineBellEnvelope* getInstance();
    static RaisedCosineBellEnvelope* setSampleRate(int sampleRate);
    static RaisedCosineBellEnvelope* setMainLobeWidth(int mainLobeWidth);
    static RaisedCosineBellEnvelope* setDuration(int duration);


private:
    RaisedCosineBellEnvelope();
    RaisedCosineBellEnvelope(int sampleRate);
    RaisedCosineBellEnvelope(int duration, int sampleRate);
    RaisedCosineBellEnvelope(int duration, int sampleRate, float mainLobeWidth);

    ~RaisedCosineBellEnvelope();

    void filterCreation();

    static RaisedCosineBellEnvelope* instance;
};
