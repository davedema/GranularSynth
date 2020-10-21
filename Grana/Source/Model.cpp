/*
  ==============================================================================

    Model.cpp
    Created: 20 Oct 2020 10:56:03pm
    Author:  jacop

  ==============================================================================
*/

#include "Model.h"

Model::Model()
{
    this->isPlaying = false;
    this->hasGranulatedCloud = false;
    this->filePos = 0;
    this->randomPos = 0.5;
    this->sectionSize = 50.;
    this->density = 25.;
    this->grainSize = 25.;
    this->speed = 1;
}

void Model::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == "filepos") {
        this->newFilePos = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::filepos);
    }
    else if (parameterID == "randompos") {
        this->newRandomPos = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::randomPos);
    }
    else if (parameterID == "isPlaying") {
        this->isPlaying = (bool)newValue;
    }
    else if (parameterID == "Section Size") {
        this->newSectionSize = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::sectionSize);
    }
    else if (parameterID == "Density") {
        this->newDensity = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::density);
    }
    else if (parameterID == "Grain Size") {
        this->grainSize = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::grainSize);
    }
    else if (parameterID == "Speed") {
        this->speed = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::speed);
    }
}

float Model::getFilePos()
{
    return this->filePos;
}

float Model::getRandomPos()
{
    return this->randomPos;
}

float Model::getSectionSize()
{
    return this->sectionSize;
}

float Model::getDensity()
{
    return this->density;
}

float Model::getGrainSize()
{
    return this->grainSize;
}

float Model::getSpeed()
{
    return this->speed;
}

bool Model::getIsPlaying()
{
    return this->isPlaying;
}

void Model::setHasGranulatedCloud(bool hasDone)
{
    this->hasGranulatedCloud = hasDone;
}

bool Model::getHasGranulatedCloud()
{
    return this->hasGranulatedCloud;
}

bool Model::detectChange(UserControls parameter)
{
    return changedParameters.contains(parameter);
}

bool Model::detectAnyChange()
{
    return !changedParameters.isEmpty();
}
