/*
  ==============================================================================

    FileLoader.cpp
    Created: 21 Aug 2020 4:45:05pm
    Author:  daveg

  ==============================================================================
*/

#include "FileLoader.h"



FileLoader* FileLoader::instance = 0;

FileLoader::FileLoader(){
    formatManager = new juce::AudioFormatManager();
   // transportSource = new juce::AudioTransportSource();
    readerSource = new std::unique_ptr<juce::AudioFormatReaderSource>();

    thumbnailCache = new juce::AudioThumbnailCache(5);
    thumbnail = new juce::AudioThumbnail(512, *formatManager, *thumbnailCache);
    buffer = new juce::AudioBuffer<float>();

}


FileLoader::~FileLoader() {
    delete formatManager;
   // delete transportSource;
    delete readerSource;
    delete thumbnailCache;
    delete thumbnail;
    delete instance;
}

FileLoader* FileLoader::getInstance()
{
    if (!instance)
        instance = new FileLoader();
    return instance;
}


void FileLoader::loadWaveform(juce::File file)
{

    juce::AudioFormatReader* reader = formatManager->createReaderFor(file);
    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
    //transportSource->setSource(newSource.get(), 0, nullptr, reader->sampleRate);                                                                                                            // [13]
    thumbnail->setSource(new juce::FileInputSource(file));
    readerSource->reset(newSource.release());
    buffer->setSize(reader->numChannels, reader->lengthInSamples);
    reader->read(buffer, 0, reader->lengthInSamples, 0, true, true);

    buffer;

}




void FileLoader::loadFile(const juce::String& gpath)
{
    auto file = juce::File(gpath);
    auto* formatReader = formatManager->createReaderFor(file);

    if (formatReader != nullptr) {
        loadWaveform(file);
        //mformatReader.read(actual_sample);
    }
}

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



