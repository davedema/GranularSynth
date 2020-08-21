/*
  ==============================================================================

    FileLoader.h
    Created: 21 Aug 2020 4:45:05pm
    Author:  daveg

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

class FileLoader {



private:

    static FileLoader* instance;

    juce::AudioFormatManager *formatManager;
   // juce::AudioTransportSource *transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> *readerSource;

    juce::AudioThumbnailCache* thumbnailCache;                  // [1]
    juce::AudioThumbnail* thumbnail;


    juce::AudioBuffer<float> *buffer;






public:

    
    FileLoader();
    ~FileLoader();

    static FileLoader* getInstance();

    void loadWaveform(juce::File file);

    // DRAG AND DROP 
    void loadFile(const juce::String& gpath);

    // GETTERS AND SETTERS
    juce::AudioFormatManager *getFormatManager() const;

    juce::AudioTransportSource *getTransportSource() const;
 
    std::unique_ptr<juce::AudioFormatReaderSource> *getReaderSource() const;

    juce::AudioThumbnailCache* getThumbnailCache() const;

    juce::AudioThumbnail* getThumbnail() const;

    juce::AudioBuffer<float> *getAudioBuffer() const;





};