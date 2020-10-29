/*
  ==============================================================================

    Granulator.cpp
    Created: 20 Sep 2020 11:16:43am
    Author:  gabri

  ==============================================================================
*/
#include "Granulator.h"

Granulator::Granulator()
{
    this->activeGrains.clearQuick();
    this->model = nullptr;
    this->nextOnset = 0;
    this->position = 0;
    this->processorSampleRate = 0;
    this->portionLength = 0;
}

Granulator::~Granulator()
{
    for (auto grain : this->activeGrains) {
        this->activeGrains.remove(this->activeGrains.indexOf(grain));
        delete grain;
    }
}

//Initialize everything, add the first grain and take the onSet time for the next grain. Called when PLAY button is clicked
void Granulator::initialize(int portionLength)
{
    this->activeGrains.clearQuick();
    this->position = this->model->getFilePos();
    this->activeGrains.add(new Grain(this->model->getGrainSize(), 
                                     this->position, 
                                     false, 
                                     0,
                                     this->model->getEnvIndex(),
                                     this->model->getEnvWidth()));
    this->nextOnset = round(this->processorSampleRate/this->model->getDensity());
    this->portionLength = portionLength;
}


// Process the sound
void Granulator::process(AudioBuffer<float>& outputBuffer, int numSamples)
{
    for (int samplePos = 0; samplePos < numSamples; samplePos++) {

        //If there are no active grains put 0 on output buffer
        if (this->activeGrains.isEmpty()) {
            for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
                outputBuffer.addSample(i, samplePos, 0);
            }
        }
        else {
            //Cycles through the active grains, if the grain is finished remove it otherwise play the current sample
            for (auto grain : this->activeGrains) {
                if (grain->isFinished()) {
                    this->activeGrains.remove(this->activeGrains.indexOf(grain));
                    delete grain;
                }
                else {
                    for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
                        outputBuffer.addSample(i, samplePos, grain->getCurrentSample(i));   //Should add the sample already envelopped
                    }
                    grain->updateIndex();
                }
            }
        }

        //Increment the position in the audio file, if it's at the end of the portion get back to the starting pos
        this->position++;
        if ((this->position < this->model->getFilePos()) || (this->position >= (this->model->getFilePos() + this->model->getSectionSize()))) {
            this->position = this->model->getFilePos();
        }

        //Decrement the next onset time, if it's 0 add a new grain and get the next one
        this->nextOnset--;
        if (this->nextOnset == 0) {
            this->activeGrains.add(new Grain(this->model->getGrainSize(), 
                                             this->position,
                                             false, 
                                             200,
                                             this->model->getEnvIndex(),
                                             this->model->getEnvWidth()));
            this->nextOnset = round(this->processorSampleRate / this->model->getDensity());
        }
    }
}

void Granulator::setModel(Model* model)
{
    this->model = model;
    this->strategy.setModel(model);
}

void Granulator::setProcessorSampleRate(double processorSampleRate)
{
    this->processorSampleRate = processorSampleRate;
    this->strategy.setSampleRate(processorSampleRate);
}
