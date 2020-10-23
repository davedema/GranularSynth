/*
  ==============================================================================

    GaussianEnvelope.h
    Created: 22 Sep 2020 5:26:57pm
    Author:  jacop

  ==============================================================================
*/

#define _USE_MATH_DEFINES
#pragma once
#include <cmath> 
#include "GrainEnvelope.h"
#include <iomanip> 
#include <vector>






class GaussianEnvelope : public GrainEnvelope
{
public:

    static void reset();
    static void reset(int duration, int sampleRate, float mainLobeWidth);
    static GaussianEnvelope* setDuration(int duration);
    static GaussianEnvelope* setSampleRate(int sampleRate);
    static GaussianEnvelope* setMainLobeWidth(int mainLobeWidth);
    static GaussianEnvelope* getInstance();
    
    
    

private:

    GaussianEnvelope();
    GaussianEnvelope(int sampleRate);
    GaussianEnvelope(int duration, int sampleRate);
    GaussianEnvelope(int duration, int sampleRate, float mainLobeWidth);
    
    void filterCreation();

    ~GaussianEnvelope();

    static GaussianEnvelope* instance;
};
