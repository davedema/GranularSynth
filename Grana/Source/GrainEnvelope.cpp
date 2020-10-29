/*
  ==============================================================================

    GrainEnvelope.cpp
    Created: 24 Sep 2020 5:22:50pm
    Author:  Davide De Marco

  ==============================================================================
*/

#include "GrainEnvelope.h"


float GrainEnvelope::getEnvelopeValue(int index, int selectedEnv, int length, float mainlobewidth)
{
    switch (selectedEnv)
    {
    case(1):
		return getGaussian(index, length, mainlobewidth);
        break;

    case(2):
		return getRaisedCosine(index, length, mainlobewidth);
        break;

    case(3):
		return getTrapezoidal(index, length, mainlobewidth);
        break;
    }
    return float();
}


float GrainEnvelope::getGaussian(int index, int duration, float mainLobeWidth)
{
	if (mainLobeWidth == 0)
		return index == duration / 2 ? 1 : 0;
	int halfDuration = (int)duration / 2;
	int halfDurationPositive = halfDuration;
	float alpha = duration /  (1 - 1/mainLobeWidth);

	if (duration % 2 == 0) { //this is done in order to solve the problem of having an array
								 //1 cell longer than needed when working with a even duration
		halfDurationPositive--;
	}
	float stdv = (duration - 1) / (2 * alpha);

	index = index - halfDurationPositive;
	return exp(-(index * index) / (2 * (alpha * alpha)));
}


float GrainEnvelope::getRaisedCosine(int index, int duration, float mainLobeWidth)
{
	int sustain = mainLobeWidth * duration;
	int attack = (duration - sustain) / 2;


	if (index < attack)
		return (1 + cos(PI + PI * index / attack)) / 2;
	if (index < sustain + attack)
		return 1;
	index -= sustain + attack;  //shift
	return (1 - cos(PI - PI * index / attack)) / 2;  //index  ---> -1 * index

}

float GrainEnvelope::getTrapezoidal(int index, int duration, float mainLobeWidth)
{
	int sustain = mainLobeWidth * duration;
	int attack = (duration - sustain) / 2;
	if (attack == 0)
		return 1;

	float angularCoeff = 1.0f / attack;

	if (index <= attack)
		return angularCoeff * index;
	if (index < sustain + attack)
		return 1;
	index -= sustain + attack;  //shift
	return 1 - angularCoeff * index;

}