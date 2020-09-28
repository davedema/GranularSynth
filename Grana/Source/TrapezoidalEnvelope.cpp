/*
  ==============================================================================

    TrapezoidalEnvelope.cpp
    Created: 22 Sep 2020 5:27:29pm
    Author:  jacop

  ==============================================================================
*/

#include "TrapezoidalEnvelope.h"


TrapezoidalEnvelope::TrapezoidalEnvelope(int sampleRate) : duration(0), sampleRate(sampleRate), mainLobeWidth(0.95)
{
	filterCreation();
}

TrapezoidalEnvelope::TrapezoidalEnvelope(float duration, int sampleRate) : duration(duration), sampleRate(sampleRate), mainLobeWidth(0.95) {
	filterCreation();
}

TrapezoidalEnvelope::TrapezoidalEnvelope(float duration, int sampleRate, float mainLobeWidth) : duration(duration), sampleRate(sampleRate), mainLobeWidth(0.95) {
	filterCreation();
}





float TrapezoidalEnvelope::currentValue(float time)
{
	return kernel[(int)time * sampleRate];
}


// C++ prgroam to generate Gaussian filter 
// Function to create Gaussian filter 
void TrapezoidalEnvelope::filterCreation()
{
	int sampleLength = duration * sampleRate;
	int sustain = mainLobeWidth * sampleLength;
	int attack = (sampleLength - sustain) / 2;

	// generating kernel (attack)

	float angularCoeff = 1/attack;

	for (int x = 0; x < attack; x++) {
		kernel.push_back(angularCoeff* (float)x);
	}


	for (int x = 0; x < sustain; x++) {
		kernel.push_back(1);
	}

	for (int x = attack - 1; x >= 0; x--) {
		kernel.push_back(kernel[x]);
	}
}

