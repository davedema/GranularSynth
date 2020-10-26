/*
  ==============================================================================

    GrainCloud.cpp
    Created: 22 Sep 2020 5:20:42pm
    Author:  jacop

  ==============================================================================
*/
#include "GrainCloud.h"


GrainCloud::GrainCloud()
{
    this->highresolution = false;
    this->grains.clearQuick();
    this->filePos = 0;
    this->sectionSize = 0;
    this->model = nullptr;

}

GrainCloud::~GrainCloud()
{
    for (auto g : grains) {
        delete g;
    }
    //this->grains.clear();
}

void GrainCloud::addGrain(Grain* g)
{
    grains.add(g);
    //g->equalTemperament();
}

void GrainCloud::deleteGrain(Grain* g)
{
    grains.remove(&g);
}

void GrainCloud::granulatePortion(int filePosition, int grainLength, int portionLength) 
{
    int hopSize = grainLength / 2; //COLA condition
    int numGrains = portionLength / hopSize; // # of grains
    int fileLength = FileLoader::getInstance()->getAudioBuffer()->getNumSamples();
    while (filePosition + numGrains * hopSize + grainLength > fileLength) // check if outside
        numGrains--;
    for (int i = 0; i < numGrains - 1; i++) { //add grains
        Grain* grain = new Grain(grainLength, filePosition + i * hopSize, highresolution);
        addGrain(grain);
    }

}

void GrainCloud::shiftPortion(int shift)
{

}

int GrainCloud::getCloudLength()
{
    return this->grains.size();
}

Grain* GrainCloud::getNextGrain(Grain* currentGrain)
{
    /*
    if (currentGrain == nullptr) return grains.getFirst();

    int idx = grains.indexOf(currentGrain);
    if (idx != -1) return grains.getUnchecked((idx + 1)%grains.size());

    return grains.getFirst();

    */
    Grain* nextGrain = nullptr;
    if (model->getHasLoadedFile())
    {
        nextGrain = new Grain(round(model->getGrainSize() * FileLoader::getInstance()->getSampleRate() / 1000),
            round(model->getFilePos() * FileLoader::getInstance()->getAudioBuffer()->getNumSamples() / 100),
            this->highresolution);
           
    }

    return nextGrain;

}

void GrainCloud::setHighResolution(bool highResolution)
{
    this->highresolution = highResolution;
}

void GrainCloud::setModel(Model* model)
{
    this->model = model;
}

void GrainCloud::setfilePos()
{
    this->filePos = this->model->getFilePos();
}

void GrainCloud::setsectionSize()
{
    this->sectionSize = this->model->getSectionSize();

}

void GrainCloud::setFileLoader(FileLoader* fileLoader)
{
    this->fileLoader = fileLoader;
}
