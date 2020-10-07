/*
  ==============================================================================

    GrainEnvelope.h
    Created: 22 Sep 2020 5:21:55pm
    Author:  jacopo

  ==============================================================================
*/

#pragma once
#include <vector>



class GrainEnvelope
{

public:
	
    float currentValue(float time);
    std::vector<double> getKernel();

    static void reset();
    static void reset(float duration, int sampleRate, float mainLobeWidth);
    
protected:

    float duration; //duration in seconds
    int sampleRate;
    float mainLobeWidth;
    std::vector<double> kernel;

private:
    
    
    
};

