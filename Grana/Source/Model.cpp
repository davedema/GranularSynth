/*
  ==============================================================================

    Model.cpp
    Created: 20 Oct 2020 10:56:03pm
    Author:  jacop

  ==============================================================================
*/

#include "Model.h"
#include "FileLoader.h"

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
    this->speedModule = 1;
    this->sampleRate = 0;
    this->fileLength = 0;
    this->speedDirection = 1;
    this->readposition = 0;
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
        this->speedModule = abs(newValue);  //speed module
        this->speedDirection = newValue == 0 ? 0 : (newValue /abs(newValue)); // 1 or -1 for now
    }

    else if (parameterID == "envIndex")
    {
        this->envIndex = newValue;
    }
}

int Model::getFilePos()
{
    return round(this->filePos*this->fileLength);
}

float Model::getEnvWidth()
{
    return this->envWidth;
}

int Model::getSectionSize()
{
    return round(this->sectionSize*this->fileLength);
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
    return pow(10, -3) * this->grainSize * this->sampleRate;
}

float Model::getSpeedModule()
{
    return this->speedModule;
}

bool Model::getIsPlaying()
{
    return this->isPlaying;
}

void Model::setHasLoadedFile(bool hasDone, int fileLength)
{
    this->hasLoadedFile = hasDone;
    this->fileLength = fileLength;
}

bool Model::getHasLoadedFile()
{
    return this->hasLoadedFile;
}

void Model::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}

int Model::getSpeedDirection()
{
    return this->speedDirection;
}

int Model::getReadPosition()
{
    return this->readposition;
}

void Model::setReadPosition(int readPosition)
{
    this->readposition = readPosition;
}


Array<Point<float>>* Model::getxyPlane()
{
    return &xyPlane;
}

int Model::getCurrentPosition()
{
    int pos = round(jmap<int>(readposition, 0,
                              sectionSize * FileLoader::getInstance()->getAudioBuffer()->getNumSamples(),
                              0, xyPlane.size() ));
    float ret = xyPlane[pos].getX() * sectionSize;
    return ret ;
}

float Model::getCurrentFrequencyShift()
{
    int pos = round(jmap<int>(readposition, 0,
                              sectionSize * FileLoader::getInstance()->getAudioBuffer()->getNumSamples(),
                              0, xyPlane.size()));
    float freq = xyPlane[pos].getY() * 2000.0f - 1000.0f;
    return freq;
}
