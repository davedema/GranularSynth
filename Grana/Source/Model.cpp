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
    this->hasLoadedFile = false;
    this->filePos = 0;
    this->envIndex = 1;
    this->envAmt = 0.5;
    this->sectionSize = 50.;
    this->density = 25.;
    this->grainSize = 25.;
    this->speed = 1;
}

void Model::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == "filepos") {
        this->filePos = newValue;
    }
    else if (parameterID == "envAmt") {
        this->newenvAmt = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::envAmt);

    }
    else if (parameterID == "isPlaying") {
        this->isPlaying = (bool)newValue;
    }
    else if (parameterID == "Section Size") {
        this->sectionSize = newValue;
    }
    else if (parameterID == "Density") {
        this->density = newValue;
    }
    else if (parameterID == "Grain Size") {
        this->grainSize = newValue;
    }
    else if (parameterID == "Speed") {
        this->speed = newValue;
    }

    else if (parameterID == "envIndex")
    {
        this->envIndex = newValue;
        changedParameters.addIfNotAlreadyThere(UserControls::envIndex);
    }
}

float Model::getFilePos()
{
    return this->filePos;
}

float Model::getenvAmt()
{
    return this->envAmt;
}

float Model::getSectionSize()
{
    return this->sectionSize;
}

float Model::getDensity()
{
    //return this->density;
    return 10;
}

int Model::getEnvIndex()
{
    return this->envIndex;
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

void Model::setHasLoadedFile(bool hasDone)
{
    this->hasLoadedFile = hasDone;
}


bool Model::getHasLoadedFile()
{
    return this->hasLoadedFile;
}

bool Model::detectChange(UserControls parameter)
{
    return changedParameters.contains(parameter);
}

bool Model::detectAnyChange()
{
    return !changedParameters.isEmpty();
}
