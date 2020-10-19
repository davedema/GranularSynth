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
    this->grains.clearQuick();
}

GrainCloud::~GrainCloud()
{
    /*for (auto g : grains) {
        delete g;
    }*/
    this->grains.clear();
}

void GrainCloud::addGrain(Grain* g)
{
    grains.add(g);
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
        Grain* grain = new Grain(grainLength, filePosition + i * hopSize);
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
    if (currentGrain == nullptr) return grains.getFirst();

    int idx = grains.indexOf(currentGrain);
    DBG("Number of grains: " + std::to_string(grains.size()));
    DBG("Grain index: " + std::to_string(idx));
    if (idx != -1) return grains.getUnchecked(idx + 1);

    return grains.getFirst();

}
