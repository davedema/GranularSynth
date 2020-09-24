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


class GaussianEnvelope : private GrainEnvelope
{
public:
	GaussianEnvelope(int sampleRate);
    GaussianEnvelope(float duration, int sampleRate); 
    GaussianEnvelope(float duration, int sampleRate, float mainLobeWidth);
    float currentValue(float time) override;
    virtual float nextValue() override;

    void FilterCreation();

	~GaussianEnvelope();

private:
    float duration; //duration in seconds
    int sampleRate;
    float mainLobeWidth;
    std::vector<double> GKernel;
};