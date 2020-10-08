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
	
    float currentValue(int time);

    static void reset();
    static void reset(int duration, int sampleRate, float mainLobeWidth);

    std::vector<double> kernel;
    
protected:

    int duration; //duration in samples
    int sampleRate;
    float mainLobeWidth;
    

private:
    
    
    
};

