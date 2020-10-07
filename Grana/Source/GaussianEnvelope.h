/*
  ==============================================================================

    GaussianEnvelope.h
    Created: 22 Sep 2020 5:26:57pm
    Author:  jacop

  ==============================================================================
*/

#define _USE_MATH_DEFINES
#include <cmath> 
#include "GrainEnvelope.h"
#include <iomanip> 
#include <vector>



#pragma once


class GaussianEnvelope : public GrainEnvelope
{
public:

    static void reset();
    static void reset(float duration, int sampleRate, float mainLobeWidth);
    static GaussianEnvelope* setDuration(int duration);
    static GaussianEnvelope* setSampleRate(int sampleRate);
    static GaussianEnvelope* setMainLobeWidth(int mainLobeWidth);
    static GaussianEnvelope* getInstance();
    
    

private:

    GaussianEnvelope();
    GaussianEnvelope(int sampleRate);
    GaussianEnvelope(float duration, int sampleRate);
    GaussianEnvelope(float duration, int sampleRate, float mainLobeWidth);
    
    void filterCreation();

    ~GaussianEnvelope();

    static GaussianEnvelope* instance;
};
