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
    this->sectionSize = 0.5;
    this->density = 25.;
    this->grainSize = 25.;
    this->speedModule = 1;
    this->sampleRate = 0;
    this->fileLength = 0;
    this->speedDirection = 1;
    this->readposition = 0;
    this->init = false;
    this->currentGain = 0.75;
    this->spread = 0;
    this->sampleRate = 0;
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
        this->speedDirection = newValue == 0 ? 0 : (newValue / abs(newValue)); // 1 or -1 for now
    }
    else if (parameterID == "envIndex")
    {
        this->envIndex = newValue;
    }
    else if (parameterID == "masterGain")
    {
        this->currentGain = Decibels::decibelsToGain(newValue);
    }
    else if (parameterID == "Random Spread")
    {
        this->spread = newValue * this->fileLength/2 * 0.5;
    }
    else if (parameterID == "Random")
    {
        this->random = newValue;
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

int Model::getGrainSize()
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

void Model::setIsPlaying(bool val)
{
    this->isPlaying = val;
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

bool Model::getInit()
{
    return this->init;
}

void Model::setInit(bool val)
{
    this->init = val;
}

int Model::getReadPosition()
{
    return this->readposition;
}

int Model::getRealPosition()
{
    return this->realPosition;
}

void Model::setRealPosition(int newPos)
{
    this->realPosition = newPos;
}

void Model::setReadPosition(int readPosition)
{
    this->readposition = readPosition;
}


Array<Point<float>>* Model::getxyPlane()
{
    return &xyPlane;
}

int Model::getxyArrayPosition()
{
    if (!getHasLoadedFile() || !getIsPlaying())
        return 0;

    float position = (float)abs(readposition - filePos * FileLoader::getInstance()->getAudioBuffer()->getNumSamples()) * //value to map
        (float)xyPlane.size() /  //new range
        ((sectionSize ) * (float)FileLoader::getInstance()->getAudioBuffer()->getNumSamples()); //old range
    return position;
}

Point<float> Model::getCurrentxyPosition()
{
    int pos = getxyArrayPosition();
    auto ret = xyPlane[pos];
    return ret;
}

float Model::getCurrentFrequencyShift()
{
    if (xyPlane.isEmpty())
        return 0;
    int pos = getxyArrayPosition();
    float freq = jmax(0.0f, jmin(xyPlane[pos].getY(), 1.0f)) * 4000.0f - 2000.0f;
    return freq;
}

int Model::getCurrentTime()
{
    if (xyPlane.isEmpty())
        return this->readposition;

    int pos = getxyArrayPosition();
    float currentTime = (this-> filePos * FileLoader::getInstance()->getAudioBuffer()->getNumSamples()) 
                        + jmax(0.0f, jmin(xyPlane[pos].getX(), 1.0f)) * this->sectionSize
                        * FileLoader::getInstance()->getAudioBuffer()->getNumSamples() ;
    return (int)jmin(currentTime, (float)fileLength - 1); // in samples
}

float Model::getCurrentGain()
{
    return this->currentGain;
}

int Model::getSpread()
{
    if (this->spread == 0) return 0;
    return r_spread.nextInt(Range<int>(-this->spread, this->spread));
}

bool Model::randomize()
{
    return ((r_random.nextFloat() * 100) < this->random);
}


int Model::getSampleRate()
{
    return this->sampleRate;
}

void Model::setSampleRate(int sampleRate)
{
    this->sampleRate = sampleRate;
}

float Model::getInstantaneousFrequency()
{
    return FileLoader::getInstance()->getOriginalInstantaneousFrequency(this->getRealPosition());
}
