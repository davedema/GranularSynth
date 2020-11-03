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
                                     this->model->getSpeedDirection()));
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
        featureExtractor->pushSample(toExtract / 2);  // pass the average between two channels to extractor

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

            Grain* toAdd = new Grain(this->model->getGrainSize(),
                                     readPosition,
                                     false,
                                     model->getCurrentFrequencyShift(),
                                     this->model->getEnvIndex(),
                                     this->model->getEnvWidth(),
                                     this->processorSampleRate,
                                     this->model->getSpeedDirection());

            //If there are active grains do SOLA
            if (!this->activeGrains.isEmpty()) 
            {
                //int lag = this->computeLag(this->activeGrains.getLast()->getBuffer(), toAdd->getBuffer(), this->nextOnset, this->model->getGrainSize());
                int lag = 0;
                int crossfade = this->activeGrains.getLast()->remainingLife();
                toAdd->setLag(lag);
                toAdd->applyCrossFade(crossfade, true);
                this->activeGrains.getLast()->applyCrossFade(crossfade, false);
            }

            this->activeGrains.add(toAdd);  
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

int Granulator::computeLag(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength)
{
    int lag = 0;
    Array<float>* correlationArray = computeCrossCorrelation(
        currentBuffer,
        nextBuffer,
        userLength,
        grainLength
    ); //compute correlation
    float* maxvalue = std::max_element(
        correlationArray->begin(),
        correlationArray->end());
    lag = round(std::distance(correlationArray->begin(), std::max_element(
        correlationArray->begin(),
        correlationArray->end()))); //add lag

    return lag;
}

Array<float>* Granulator::computeCrossCorrelation(AudioBuffer<float>* currentBuffer, AudioBuffer<float>* nextBuffer, int userLength, int grainLength)
{
    Array<float>* correlationArray = new Array<float>();
    int numChannels = currentBuffer->getNumChannels();
    correlationArray->clear();

    //begin compute autocorrelation
    for (int i = 0; i < grainLength - userLength; i++) {
        float totalValue = 0;
        float newValue = 0;
        for (int j = userLength; j < grainLength - i; j++) {
            newValue += currentBuffer->getSample(0, j) * nextBuffer->getSample(0, j - userLength);
        }
        newValue /= (float)(grainLength - userLength);
        totalValue += newValue;
        totalValue /= numChannels; //average over channels
        correlationArray->add(totalValue);
    }
    //end compute autocorrelation
    return correlationArray;
}

