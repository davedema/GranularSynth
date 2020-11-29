/*
  ==============================================================================

    FileLoader.cpp
    Created: 21 Aug 2020 4:45:05pm
    Author:  daveg

  ==============================================================================
*/

#include "FileLoader.h"



FileLoader* FileLoader::instance = 0;

// Constructor
FileLoader::FileLoader()
{
    hilbertTransform = nullptr;
    formatManager = new juce::AudioFormatManager();
    readerSource = new std::unique_ptr<juce::AudioFormatReaderSource>();
    thumbnailCache = new juce::AudioThumbnailCache(5);
    thumbnail = new juce::AudioThumbnail(512, *formatManager, *thumbnailCache);
    buffer = new juce::AudioBuffer<float>();
    this->hostRate = 44100;
    this->ceiledLength = 0;
}

// Destructor
FileLoader::~FileLoader() 
{
    delete formatManager;
    readerSource->reset(nullptr);
    delete thumbnailCache;
    delete thumbnail;
    delete buffer;
    if(hilbertTransform != nullptr)
        free(hilbertTransform);
}

// Get the instance of FileLoader
FileLoader* FileLoader::getInstance()
{
    if (!instance)
        instance = new FileLoader();
    return instance;
}

void FileLoader::resetInstance()
{
    if (instance != nullptr) {
        delete instance; // REM : it works even if the pointer is NULL (does nothing then)
        instance = nullptr; // so GetInstance will still work.
    }
}

// Read the file and loads it in the thumbnail (to display) and in the buffer
void FileLoader::loadWaveform(juce::File file)
{
    auto* reader = formatManager->createReaderFor(file);
    AudioSampleBuffer temp, waveBuffer;
    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));                                                                                                          // [13]
    thumbnail->setSource(new juce::FileInputSource(file));
    readerSource->reset(newSource.release());
    buffer->setSize(reader->numChannels, reader->lengthInSamples);


    double ratio = reader->sampleRate / this->hostRate;

    temp.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
    waveBuffer.setSize((int)reader->numChannels, (int)(((double)reader->lengthInSamples) / ratio));

    reader->read(&temp, 0, (int)reader->lengthInSamples, 0, true, true);

    ScopedPointer<LagrangeInterpolator> resampler = new LagrangeInterpolator();

    const float** inputs = temp.getArrayOfReadPointers();
    float** outputs = waveBuffer.getArrayOfWritePointers();
    for (int c = 0; c < waveBuffer.getNumChannels(); c++)
    {
        resampler->reset();
        resampler->process(ratio, inputs[c], outputs[c], waveBuffer.getNumSamples());
    }

    *this->buffer = AudioBuffer<float>(waveBuffer);


    ceiledLength = pow(2, ceil(log2(buffer->getNumSamples())));
    hilbertTransform = (double*)calloc((size_t)(buffer->getNumChannels() * (size_t)2 * ceiledLength), sizeof(double));

    for (int i = 0; i < this->buffer->getNumChannels(); i++) {
        for (int j = 0; j < buffer->getNumSamples(); j++) { //apply envelope
            if (hilbertTransform != NULL) {
                hilbertTransform[i * 2 * ceiledLength + j * 2] = buffer->getSample(i, j);
                hilbertTransform[i * 2 * ceiledLength + j * 2 + 1] = 0; //real signal ----> a value every two set to zero
            }
        }

        if (hilbertTransform != NULL)
            hilbert(&hilbertTransform[i * ceiledLength], ceiledLength); //Transform file
    }
}

// Drag & Drop funztion (calls loadWaveform to load the file)
void FileLoader::loadFile(const juce::String& gpath)
{
    auto file = juce::File(gpath);
    auto* formatReader = formatManager->createReaderFor(file);
    if (formatReader != nullptr) {
        loadWaveform(file);
    }
}

// GETTERS

juce::AudioFormatManager *FileLoader::getFormatManager() const
{
    return formatManager;
}

std::unique_ptr<juce::AudioFormatReaderSource> *FileLoader::getReaderSource() const
{
    return readerSource;
}

juce::AudioThumbnailCache* FileLoader::getThumbnailCache() const
{
    return thumbnailCache;
}

juce::AudioThumbnail* FileLoader::getThumbnail() const
{
    return thumbnail;
}

juce::AudioBuffer<float> *FileLoader::getAudioBuffer() const
{
    return buffer;
}


double* FileLoader::getHilbertTransform()
{
    return this->hilbertTransform;
}

int FileLoader::getCeiledLength()
{
    return this->ceiledLength;
}

void FileLoader::setHostRate(double hostRate)
{
    this->hostRate = hostRate;
}

int FileLoader::bufferHilbertIndex(int channel, int index)
{
    return 2 * (channel * this->getCeiledLength() + index);
}

float FileLoader::getPhase(int time)
{
    int channels = this->buffer->getNumChannels();
    int phase = 0;
    for (int i = 0; i < channels; i++)
        phase += atan2f(this->hilbertTransform[bufferHilbertIndex(i, time) + 1], this->hilbertTransform[bufferHilbertIndex(i, time)]);
    return phase / (float)channels;
}

float FileLoader::getOriginalInstantaneousFrequency(int currentTime)
{
    if(currentTime != 0)
        return (getPhase(currentTime) - getPhase(currentTime - 1)) * this->hostRate;
    else
        return (getPhase(currentTime) - getPhase(this->buffer->getNumSamples())) * this->hostRate;
}
