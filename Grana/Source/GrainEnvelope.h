/*
  ==============================================================================

    GrainEnvelope.h
    Created: 22 Sep 2020 5:21:55pm
    Author:  jacopo

  ==============================================================================
*/

#pragma once
#include"JuceHeader.h"
#include "./Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/math_const.h"


class GrainEnvelope
{

public:
	
    static float getEnvelopeValue(int index, int selectedEnv, int length, int mainlobewidth); //sample index, selected envelope type, grain length, main lobe width  
    
protected:    

private:
    static float getGaussian(int index, int length, float mainLobeWidth);
    static float getTrapezoidal(int index, int length, float mainLobeWidth);
    static float getRaisedCosine(int index, int length, float mainLobeWidth);
    
};

