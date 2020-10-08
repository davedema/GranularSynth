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
#include "GaussianEnvelope.h"
#include "RaisedCosineBellEnvelope.h"
#include "TrapezoidalEnvelope.h"

enum class EnvType {raisedCosineBell, gaussian, trapezoidal};

class Grain {


private:

    /**long long onset;
    float lengthRecip;
    float amp;**/

    int length;
    int startPosition;
    float playbackRate; //siamo sicuri?

    FileLoader* fileLoader;
    AudioBuffer<float>* buffer;
    GrainEnvelope* envelope;

public:
    Grain(int length, int startPos);
    ~Grain();

    AudioBuffer<float>* processBuffer();


    inline float cubicinterp(float x, float y0, float y1, float y2, float y3);
    /**void process(AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples,
                int fileNumSamples, long long int time);**/
    void changeEnvelope(EnvType type);

    void activate();
    void synthesize();
};
