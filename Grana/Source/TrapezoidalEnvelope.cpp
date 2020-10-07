/*
  ==============================================================================

    TrapezoidalEnvelope.cpp
    Created: 22 Sep 2020 5:27:29pm
    Author:  jacop

  ==============================================================================
*/

#include "TrapezoidalEnvelope.h"

TrapezoidalEnvelope* TrapezoidalEnvelope::instance = 0;


TrapezoidalEnvelope::TrapezoidalEnvelope() 
{
	this->duration = 0;
	this->sampleRate = 0;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

TrapezoidalEnvelope::TrapezoidalEnvelope(int sampleRate)
{
	this->duration = 0;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

TrapezoidalEnvelope::TrapezoidalEnvelope(float duration, int sampleRate) {
	this->duration = duration;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

TrapezoidalEnvelope::TrapezoidalEnvelope(float duration, int sampleRate, float mainLobeWidth) {
	this->duration = duration;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = mainLobeWidth;
	filterCreation();
}

TrapezoidalEnvelope::~TrapezoidalEnvelope()
{
	kernel.clear();
}

TrapezoidalEnvelope* TrapezoidalEnvelope::getInstance()
{
	if (!instance)
		instance = new TrapezoidalEnvelope();
	return instance;
}


void TrapezoidalEnvelope::reset()
{
	if (instance != nullptr) {
		delete instance; // REM : it works even if the pointer is NULL (does nothing then)
		instance = nullptr; // so GetInstance will still work.
	}
}

TrapezoidalEnvelope* TrapezoidalEnvelope::setSampleRate(int sampleRate)
{
	TrapezoidalEnvelope* instance = TrapezoidalEnvelope::getInstance();
	instance->sampleRate = sampleRate;
	instance->filterCreation();
	return instance;
}

TrapezoidalEnvelope* TrapezoidalEnvelope::setDuration(int duration)
{
	TrapezoidalEnvelope* instance = TrapezoidalEnvelope::getInstance();
	instance->duration = duration;
	instance->filterCreation();
	return instance;
}

TrapezoidalEnvelope* TrapezoidalEnvelope::setMainLobeWidth(int mainLobeWidth)
{
	TrapezoidalEnvelope* instance = TrapezoidalEnvelope::getInstance();
	instance->mainLobeWidth = mainLobeWidth;
	instance->filterCreation();
	return instance;
}

void TrapezoidalEnvelope::reset(float duration, int sampleRate, float mainLobeWidth)
{
	if (instance != nullptr) {
		delete instance; // REM : it works even if the pointer is NULL (does nothing then) so GetInstance will still work.
	}
	instance = new TrapezoidalEnvelope(duration, sampleRate, mainLobeWidth);
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

