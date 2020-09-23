/*
  ==============================================================================

    Grain.h
    Created: 17 Aug 2020 11:18:28am
    Author:  daveggggggggg

  ==============================================================================
*/


#include <JuceHeader.h>
#include "FileLoader.h"
#include "GrainEnvelope.h"

enum class EnvType { RaisedCosineBell, Gaussian, Trapezoidal };

class Grain : public juce::AudioBuffer<int>{


private:

    const long long int onset;
    const int length;
    const int startPosition;
    const float lengthRecip;

    const float rate;
    const float amp;

    FileLoader* fileLoader;

    EnvType envType;


public:

    Grain(EnvType type, long long int onset, int length, int startPos, float rate, float amp) : 
        AudioBuffer(), onset(onset), length(length), startPosition(startPos),
        lengthRecip(1 / (float)length), rate(rate), amp(amp) { envType = type; fileLoader = FileLoader::getInstance();
    };

    float envelope(int time);
    inline float cubicinterp(float x, float y0, float y1, float y2, float y3);
    void process(AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples,
        int fileNumSamples, long long int time);

    EnvType getEnvType() const;
    void setEnvType(EnvType envType);

    void activate();
    void synthetize();
};
