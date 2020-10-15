/*
  ==============================================================================

    GrainCloud.h
    Created: 22 Sep 2020 5:20:42pm
    Author:  jacop

  ==============================================================================
*/

#pragma once
#include "Grain.h"
#include "SequenceStrategy.h"

class GrainCloud : public juce::SynthesiserSound { // passive class just describes the sound - the rendering happens inside the Voice class

private: 
    Array<Grain*> grains; //juce array class
    SequenceStrategy strategy;

    float grainDensity;
    int grainLength;
    float quasiSyncRange;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 engine; //Standard mersenne_twister_engine seeded with rd() and feeded to distribution
    std::uniform_real_distribution<float> distribution; //uniform distribution

public: 
    GrainCloud();
    ~GrainCloud();
    

    //virtual functions inherited from SynthesiserSound 
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;

    //custom functions
    void addGrain(Grain* g); // adds a grain to the stack
    void deleteGrain(Grain* g); // deletes a grain once its lifetime is over
    void granulatePortion(int filePosition, int grainLength, int portionLength); // granulates specified portion
    void shiftPortion(int shift);
    int getCloudLength();
    Grain* getNextGrain(Grain* currentGrain);

    //sequence strategy
    int nextInterOnset(Grain* currentGrain);


};