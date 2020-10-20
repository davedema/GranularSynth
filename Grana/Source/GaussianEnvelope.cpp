/*
  ==============================================================================

    GaussianEnvelope.cpp
    Created: 22 Sep 2020 5:26:57pm
    Author:  jacop

  ==============================================================================
*/

#include "GaussianEnvelope.h"
#include "Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/math_const.h"


GaussianEnvelope* GaussianEnvelope::instance = 0;


GaussianEnvelope::GaussianEnvelope()
{
	this->duration = 0;
	this->sampleRate = 0;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

GaussianEnvelope::GaussianEnvelope(int sampleRate)
{
	this->duration = 0;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

GaussianEnvelope::GaussianEnvelope(int duration, int sampleRate) {
	this->duration = duration;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = 0.95;
	filterCreation();
}

GaussianEnvelope::GaussianEnvelope(int duration, int sampleRate, float mainLobeWidth) {
	this->duration = duration;
	this->sampleRate = sampleRate;
	this->mainLobeWidth = mainLobeWidth;
	filterCreation();
}


GaussianEnvelope* GaussianEnvelope::getInstance()
{
	if (!instance)
		instance = new GaussianEnvelope();
	return instance;
}


void GaussianEnvelope::reset()
{
	if (instance != nullptr) {
		delete instance; // REM : it works even if the pointer is NULL (does nothing then)
		instance = nullptr; // so GetInstance will still work.
	}
}

GaussianEnvelope* GaussianEnvelope::setSampleRate(int sampleRate)
{
	GaussianEnvelope* instance = GaussianEnvelope::getInstance();
	instance->sampleRate = sampleRate;
	instance->filterCreation();
	return instance;
}

GaussianEnvelope* GaussianEnvelope::setDuration(int duration)
{
	GaussianEnvelope* instance = GaussianEnvelope::getInstance();
	instance->duration = duration;
	instance->filterCreation();
	return instance;
}

GaussianEnvelope* GaussianEnvelope::setMainLobeWidth(int mainLobeWidth)
{
	GaussianEnvelope* instance = GaussianEnvelope::getInstance();
	instance->mainLobeWidth = mainLobeWidth;
	instance->filterCreation();
	return instance;
}

void GaussianEnvelope::reset(int duration, int sampleRate, float mainLobeWidth)
{
	if (instance != nullptr) {
		delete instance; // REM : it works even if the pointer is NULL (does nothing then) so GetInstance will still work.
	}
	instance = new GaussianEnvelope(duration, sampleRate, mainLobeWidth);
}

// C++ prgroam to generate Gaussian filter 
// Function to create Gaussian filter 
void GaussianEnvelope::filterCreation()
{
	if (kernel.size() > 0) { //clear previous kernel
		kernel.clear();
	}

	int halfDuration = (int)duration / 2;
	int halfDurationPositive = halfDuration;
	float sigma_per_two = mainLobeWidth * (float)duration;
	float sigma = sigma_per_two / 2;
	float r, s = sigma * sigma;
	float triangularCoeff = pow(10, -(1 - sigma_per_two / duration) * 4);  //guessing 

	// generating kernel 

	if (duration %2 == 0) { //this is done in order to solve the problem of having an array
								 //1 cell longer than needed when working with a even duration
		halfDurationPositive--;
	}

	for (int x = -halfDuration; x <= halfDurationPositive; x++) { //windowing the window --> reference links
			kernel.push_back((exp(-(x * x) / (2 * s))) * pow(1 - abs(x) / halfDuration, triangularCoeff));
	}
}

GaussianEnvelope::~GaussianEnvelope()
{
	kernel.clear();
}



