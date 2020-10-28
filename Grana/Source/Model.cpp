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
    this->envWidth = 0.5;
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
    else if (parameterID == "envWidth") {
        this->envWidth = newValue;
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
    }
}

float Model::getFilePos()
{
    return this->filePos;
}

float Model::getEnvWidth()
{
    return this->envWidth;
}

float Model::getSectionSize()
{
    return this->sectionSize;
}

float Model::getDensity()
{
    return this->density;
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
