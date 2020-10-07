/*
  ==============================================================================

    RaisedCosineBellEnvelope.cpp
    Created: 22 Sep 2020 5:27:14pm
    Author:  jacop

  ==============================================================================
*/

#include "RaisedCosineBellEnvelope.h"

RaisedCosineBellEnvelope* RaisedCosineBellEnvelope::instance = 0;


RaisedCosineBellEnvelope::RaisedCosineBellEnvelope()
{
	this->duration = 0;
	this->sampleRate = 0;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

RaisedCosineBellEnvelope::RaisedCosineBellEnvelope(int sampleRate)
{
	this->duration = 0;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

RaisedCosineBellEnvelope::RaisedCosineBellEnvelope(float duration, int sampleRate) {
	this->duration = duration;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

RaisedCosineBellEnvelope::RaisedCosineBellEnvelope(float duration, int sampleRate, float mainLobeWidth) {
	this->duration = duration;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = mainLobeWidth;
	filterCreation();
}

RaisedCosineBellEnvelope::~RaisedCosineBellEnvelope()
{
	kernel.clear();
}


RaisedCosineBellEnvelope* RaisedCosineBellEnvelope::getInstance()
{
	if (!instance)
		instance = new RaisedCosineBellEnvelope();
	return instance;
}


void RaisedCosineBellEnvelope::reset()
{
	if (instance != nullptr) {
		delete instance; // REM : it works even if the pointer is NULL (does nothing then)
		instance = nullptr; // so GetInstance will still work.
	}
}

RaisedCosineBellEnvelope* RaisedCosineBellEnvelope::setSampleRate(int sampleRate)
{
	RaisedCosineBellEnvelope* instance = RaisedCosineBellEnvelope::getInstance();
	instance->sampleRate = sampleRate;
	instance->filterCreation();
	return instance;
}

RaisedCosineBellEnvelope* RaisedCosineBellEnvelope::setDuration(int duration)
{
	RaisedCosineBellEnvelope* instance = RaisedCosineBellEnvelope::getInstance();
	instance->duration = duration;
	instance->filterCreation();
	return instance;
}

RaisedCosineBellEnvelope* RaisedCosineBellEnvelope::setMainLobeWidth(int mainLobeWidth)
{
	RaisedCosineBellEnvelope* instance = RaisedCosineBellEnvelope::getInstance();
	instance->mainLobeWidth = mainLobeWidth;
	instance->filterCreation();
	return instance;
}

void RaisedCosineBellEnvelope::reset(float duration, int sampleRate, float mainLobeWidth)
{
	if (instance != nullptr) {
		delete instance; // REM : it works even if the pointer is NULL (does nothing then) so GetInstance will still work.
	}
	instance = new RaisedCosineBellEnvelope(duration, sampleRate, mainLobeWidth);
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

