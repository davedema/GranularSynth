/*
  ==============================================================================

    GrainCloud.h
    Created: 22 Sep 2020 5:20:42pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "Grain.h"

class GrainCloud{ // passive class just describes the sound - the rendering happens inside the Voice class

private: 
    Array<Grain*> grains; //juce array class

public: 
    GrainCloud();
    ~GrainCloud();
    void addGrain(Grain* g); // adds a grain to the stack
    void deleteGrain(Grain* g); // deletes a grain once its lifetime is over
    void granulatePortion(int filePosition, int grainLength, int portionLength); // granulates specified portion
    void shiftPortion(int shift);
    int getCloudLength();
    Grain* getNextGrain(Grain* currentGrain);
};