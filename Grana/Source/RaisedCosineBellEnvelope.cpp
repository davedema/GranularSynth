/*
  ==============================================================================

    RaisedCosineBellEnvelope.cpp
    Created: 22 Sep 2020 5:27:14pm
    Author:  jacop

  ==============================================================================
*/

#include "RaisedCosineBellEnvelope.h"


RaisedCosineBellEnvelope::RaisedCosineBellEnvelope(int sampleRate) : duration(0), sampleRate(sampleRate), mainLobeWidth(0.95)
{
}

RaisedCosineBellEnvelope::RaisedCosineBellEnvelope(float duration, int sampleRate) : duration(duration), sampleRate(sampleRate), mainLobeWidth(0.95) {}

RaisedCosineBellEnvelope::RaisedCosineBellEnvelope(float duration, int sampleRate, float mainLobeWidth) : duration(duration), sampleRate(sampleRate), mainLobeWidth(0.95) {}





float RaisedCosineBellEnvelope::currentValue(float time)
{
	return kernel[(int)time * sampleRate];
}


// C++ prgroam to generate Gaussian filter 
// Function to create Gaussian filter 
void RaisedCosineBellEnvelope::filterCreation()
{
	int sampleLength = duration * sampleRate;
	int sustain = mainLobeWidth * sampleLength;
	int attack = (sampleLength - sustain) / 2;

	// generating kernel (attack)
	for (int x = 0; x < attack; x++) {
		kernel.push_back((1 + cos(M_PI + M_PI * x / attack)) / 2);
	}


	for (int x = 0; x < sustain; x++) {
		kernel.push_back(1);
	}

	for (int x = attack-1; x >=0; x--) {
		kernel.push_back(kernel[x]);
	}
}

