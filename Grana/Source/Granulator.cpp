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
    this->hiPass.setType(dsp::LinkwitzRileyFilterType::highpass);

}

Granulator::~Granulator()
{
    for (auto grain : this->activeGrains) {       
        delete grain;
        grain = nullptr;
    }
}

//Initialize everything, add the first grain and take the onSet time for the next grain. Called when PLAY button is clicked
void Granulator::initialize(int portionLength)
{
    this->activeGrains.clearQuick();
    this->position = this->model->getFilePos();
    model->setReadPosition(0); 

    this->activeGrains.add(new Grain(this->model->getGrainSize(), 
                                     this->position, 
                                     false, 
                                     model->getCurrentFrequencyShift(),
                                     this->model->getEnvIndex(),
                                     this->model->getEnvWidth(),
                                     this->processorSampleRate,
                                     this->model->getSpeedDirection(),&this->hiPass));

    this->nextOnset = round(this->processorSampleRate/this->model->getDensity());
    this->portionLength = portionLength;
}


// Process the sound
void Granulator::process(AudioBuffer<float>& outputBuffer, int numSamples, Extractor* featureExtractor)
{
    for (int samplePos = 0; samplePos < numSamples; samplePos++) {
        float toExtract = 0;

        //If there are no active grains put 0 on output buffer
        if (this->activeGrains.isEmpty()) {
            for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
                outputBuffer.addSample(i, samplePos, 0);
            }
        }
        else 
        {
            //Cycles through the active grains, play the current sample and check if the grain is finished remove it
            for (auto grain : this->activeGrains) 
            {
                for (int i = 0; i < outputBuffer.getNumChannels(); i++) 
                {
                    outputBuffer.addSample(i, samplePos, grain->getCurrentSample(i));   //Should add the sample already envelopped
                    toExtract += grain->getCurrentSample(i);
                }
                grain->updateIndex();
                if (grain->isFinished()) 
                {
                    this->activeGrains.remove(this->activeGrains.indexOf(grain));
                    delete grain;
                }
            }
        }
        featureExtractor->pushSample(toExtract / 2 * this->model->getCurrentGain());  // pass the average between two channels to extractor

        //Increment the position in the audio file, if it's at the end of the portion get back to the starting pos
        this->position++;

        //Decrement the next onset time, if it's 0 add a new grain and get the next one
        this->nextOnset--;
        if (this->nextOnset == 0) {
            int sectionSize = jmin(model->getSectionSize(), //check not outside
                FileLoader::getInstance()->getAudioBuffer()->getNumSamples() - (int)model->getGrainSize() - model->getFilePos()); 
            sectionSize = jmax(sectionSize, (int)model->getGrainSize()); //at least 1 grain
            int timePassed = this->position - model->getFilePos(); //time passed in samples
            float readPositionShift = model->getSpeedModule() * timePassed;  //read position shift = speed * time
            int circularShift = (int)readPositionShift % sectionSize; //0 <= circularshift < section size
            int readPosition = model->getFilePos() + circularShift * model->getSpeedDirection(); //initial position + shift * direction
           
            if (model->getSpeedDirection() < 0) //if backward shift of sectionsize
                readPosition += sectionSize;

            if (readPosition == model->getFilePos()) //this is done not to increment position forever
                this->position = model->getFilePos();

            model->setReadPosition(readPosition);
            readPosition = model->getCurrentTime();
            if(readPosition > FileLoader::getInstance()->getAudioBuffer()->getNumSamples() - model->getGrainSize())
                readPosition = readPosition + model->getFilePos() - FileLoader::getInstance()->getAudioBuffer()->getNumSamples();
           
            dsp::ProcessSpec spec{ this->processorSampleRate, static_cast<juce::uint32> (this->model->getGrainSize()), FileLoader::getInstance()->getAudioBuffer()->getNumChannels() };
            this->hiPass.prepare(spec);

            // Check if we should randomize the position
            if (this->model->randomize()) {
                readPosition += this->model->getSpread();
                if (readPosition < 0)
                    readPosition += FileLoader::getInstance()->getAudioBuffer()->getNumSamples();
                else if (readPosition > FileLoader::getInstance()->getAudioBuffer()->getNumSamples() - model->getGrainSize()) {
                    readPosition += model->getGrainSize();
                    readPosition = readPosition % FileLoader::getInstance()->getAudioBuffer()->getNumSamples();
                }
            }
            this->model->setRealPosition(readPosition);
   
            Grain* toAdd = new Grain(this->model->getGrainSize(),
                                     readPosition,
                                     false,
                                     model->getCurrentFrequencyShift(),
                                     this->model->getEnvIndex(),
                                     this->model->getEnvWidth(),
                                     this->processorSampleRate,
                                     this->model->getSpeedDirection(),&this->hiPass);

            //If there are active grains do SOLA
            if (!this->activeGrains.isEmpty()) 
            {
                int crossfade = this->activeGrains.getLast()->remainingLife();
                toAdd->applyCrossFade(crossfade, true);
                this->activeGrains.getLast()->applyCrossFade(crossfade, false);               
            }
            this->nextOnset = this->processorSampleRate / model->getDensity();
            this->activeGrains.add(toAdd);             
        }
    }
}

void Granulator::setModel(Model* model)
{
    this->model = model;
}

void Granulator::setProcessorSampleRate(double processorSampleRate)
{
    this->processorSampleRate = processorSampleRate;
}

