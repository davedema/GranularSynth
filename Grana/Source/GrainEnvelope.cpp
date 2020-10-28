/*
  ==============================================================================

    GrainEnvelope.cpp
    Created: 24 Sep 2020 5:22:50pm
    Author:  Davide De Marco

  ==============================================================================
*/

#include "GrainEnvelope.h"


float GrainEnvelope::getEnvelopeValue(int index, int selectedEnv, int length, int mainlobewidth)
{
    switch (selectedEnv)
    {
    case(1):
		return getGaussian(index, length, mainlobewidth);
        break;

    case(2):
		return getTrapezoidal(index, length, mainlobewidth);
        break;

    case(3):
		return getRaisedCosine(index, length, mainlobewidth);
        break;
    }
    return float();
}


float GrainEnvelope::getGaussian(int index, int duration, float mainLobeWidth)
{

	int halfDuration = (int)duration / 2;
	int halfDurationPositive = halfDuration;
	float sigma_per_two = mainLobeWidth * (float)duration;
	float sigma = sigma_per_two / 2;
	float r, s = sigma * sigma;
	float triangularCoeff = pow(10, -(1 - sigma_per_two / duration) * 4);  //guessing 

	if (duration % 2 == 0) { //this is done in order to solve the problem of having an array
								 //1 cell longer than needed when working with a even duration
		halfDurationPositive--;
	}

	index = index - halfDurationPositive;
	return (exp(-(index * index) / (2 * s))) * pow(1 - abs(index) / halfDuration, 0.5);
}

float GrainEnvelope::getTrapezoidal(int index, int duration, float mainLobeWidth)
{
	int sampleLength = duration;
	int sustain = mainLobeWidth * sampleLength;
	int attack = (sampleLength - sustain) / 2;
	float angularCoeff;
	if (attack == 0)
		return 1;

	angularCoeff = 1 / attack;

	if (index <= attack)
		return angularCoeff * index;
	if (index < sustain)
		return 1;

	return 1 - angularCoeff * (index - sustain);

}

float GrainEnvelope::getRaisedCosine(int index, int duration, float mainLobeWidth)
{
	int sampleLength = duration;
	int sustain = mainLobeWidth * sampleLength;
	int attack = (sampleLength - sustain) / 2;


	if (index <= attack)
		return (1 + cos(PI + PI * index / attack)) / 2;
	if (index < sustain)
		return 1;

	return (cos(PI + PI * (index - sustain) / attack)) / 2;

}
