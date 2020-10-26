/*
  ==============================================================================

    GrainCloud.h
    Created: 22 Sep 2020 5:20:42pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "Grain.h"
#include "Model.h"
#include "FileLoader.h"

class GrainCloud{

private: 
    Array<Grain*> grains; //juce array class
    Model* model;
    FileLoader* fileLoader;
    bool highresolution;
    float filePos; // starting position in the sample
    float sectionSize; // size of the section to be looped

public: 
    GrainCloud();
    ~GrainCloud();
    void addGrain(Grain* g); // adds a grain to the stack
    void deleteGrain(Grain* g); // deletes a grain once its lifetime is over
    void granulatePortion(int filePosition, int grainLength, int portionLength); // granulates specified portion
    void shiftPortion(int shift);

    ///GETTERS
    int getCloudLength();
    Grain* getNextGrain(Grain* currentGrain);

    ///SETTERS
    void setHighResolution(bool highResolution);
    void setModel(Model* model);
    void setfilePos();
    void setsectionSize();
    void setFileLoader(FileLoader* fileLoader);

};