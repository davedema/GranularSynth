/*
  ==============================================================================

    FileLoader.h
    Created: 21 Aug 2020 4:45:05pm
    Author:  daveg

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

#ifdef __cplusplus 
extern "C" {
#endif
#include "./Smithsonians_Discrete_Hilbert_Fourier_Hartley_Transforms/transforms.h"

#ifdef __cplusplus 
}
#endif

class FileLoader {



private:

    static FileLoader* instance;
    juce::AudioFormatManager *formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> *readerSource;   // Original file (saved in the buffer)
    juce::AudioThumbnailCache* thumbnailCache;                      // Keep the last 5 thumbnail saved 
    juce::AudioThumbnail* thumbnail;                                // To draw the waveform
    juce::AudioBuffer<float> *buffer;                               // Contains the complete audio source (gives access to the sample)
    double* hilbertTransform;                                       //hilbert transform for each channel
    int ceiledLength;
    double hostRate;

public:

    FileLoader();                               // Constructor
    ~FileLoader();                              // Destructor
    static FileLoader* getInstance();           // Get the instance of FileLoader (SINGLETON)
	static void resetInstance();
    void loadWaveform(juce::File file);         // Load the sample in the buffer (& thumbnail)
    void loadFile(const juce::String& gpath);   // Drag & Drop function

    // GETTERS AND SETTERS
    juce::AudioFormatManager *getFormatManager() const;
    std::unique_ptr<juce::AudioFormatReaderSource> *getReaderSource() const;
    juce::AudioThumbnailCache* getThumbnailCache() const;
    juce::AudioThumbnail* getThumbnail() const;
    juce::AudioBuffer<float> *getAudioBuffer() const;
    double* getHilbertTransform();
    int getCeiledLength();
    void setHostRate(double hostRate);
};