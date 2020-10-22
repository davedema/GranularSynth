/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


// define constant values for knobs
// GRAIN DURATIONS
#define GRAIN_MIN 5.0f //in ms
#define GRAIN_MAX 100.0f //in ms

// GRAIN DENSITY
#define GRAIN_DENSITY_MIN 2.0f // in #
#define GRAIN_DENSITY_MAX 200.0f //in #


//==============================================================================
LaGranaAudioProcessor::LaGranaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    // constructors
    treeState(*this, nullptr, Identifier("CURRENT_STATE"),
        {
        //std::make_unique<AudioParameterFloat>("grain_durations", "Grain_Durations", GRAIN_MIN, GRAIN_MAX, 25.0f), // id, name, min,max, initial value
        //std::make_unique< AudioParameterFloat>("grain_density", "Grain_Density", GRAIN_DENSITY_MIN, GRAIN_DENSITY_MAX, 25.0f),
        std::make_unique<AudioParameterFloat>("filepos", "Filepos", 0, 100, 0.0f),
        std::make_unique<AudioParameterFloat>("randompos", "Randompos", 0, 1, 0.5f),
        std::make_unique<AudioParameterBool>("isPlaying", "isPlaying", false),
        std::make_unique< AudioParameterFloat>("Section Size", "Section Size", 0, 100, 50),
        std::make_unique< AudioParameterFloat>("Density", "Density", GRAIN_DENSITY_MIN, GRAIN_DENSITY_MAX, 25.0f),
        std::make_unique<AudioParameterFloat>("Grain Size", "Grain Size", GRAIN_MIN, GRAIN_MAX, 25.0f), // id, name, min,max, initial value,
        std::make_unique< AudioParameterFloat>("Speed", "Speed", -2, 2, 1)
})
#endif
{ 
    treeState.addParameterListener("filepos", &granulatorModel);
    treeState.addParameterListener("randompos", &granulatorModel);
    treeState.addParameterListener("isPlaying", &granulatorModel);
    treeState.addParameterListener("Section Size", &granulatorModel);
    treeState.addParameterListener("Density", &granulatorModel);
    treeState.addParameterListener("Grain Size", &granulatorModel);
    treeState.addParameterListener("Speed", &granulatorModel);
}

LaGranaAudioProcessor::~LaGranaAudioProcessor()
{
    FileLoader::resetInstance();
    GaussianEnvelope::reset();
    TrapezoidalEnvelope::reset();
    RaisedCosineBellEnvelope::reset();
}

//==============================================================================
const juce::String LaGranaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LaGranaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LaGranaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LaGranaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LaGranaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LaGranaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LaGranaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LaGranaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LaGranaAudioProcessor::getProgramName (int index)
{
    return {};
}

void LaGranaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LaGranaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->sampleRate = sampleRate;
}

void LaGranaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LaGranaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LaGranaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    int currentBufferLength = buffer.getNumSamples();
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, currentBufferLength);

    if (granulatorModel.getHasGranulatedCloud() && granulatorModel.getIsPlaying())
        granulator.process(buffer, buffer.getNumSamples());
    else if (granulatorModel.detectAnyChange())
        return; //trigger stuff here, easy solution to handle parameter changes
}

//==============================================================================
bool LaGranaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LaGranaAudioProcessor::createEditor()
{
    return new LaGranaAudioProcessorEditor (*this);
}

//==============================================================================
void LaGranaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void LaGranaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState* LaGranaAudioProcessor::getValueTreeState()
{
    return &treeState;
}

void LaGranaAudioProcessor::granulate()
{
    //float durationValue = treeState.getRawParameterValue("grain_durations")->load() * FileLoader::getInstance()->getSampleRate() / 1000;
    float durationValue = granulatorModel.getGrainSize() * FileLoader::getInstance()->getSampleRate() / 1000;
    int sampleDuration = (int)durationValue;
    GrainCloud* cloud = granulator.getCloud();
    float floatPos = granulatorModel.getFilePos() * FileLoader::getInstance()->getAudioBuffer()->getNumSamples() / 100;
    int filePos = (int)floatPos;
    cloud->granulatePortion(filePos, sampleDuration, FileLoader::getInstance()->getAudioBuffer()->getNumSamples());
    this->granulator.initialize();
    granulatorModel.setHasGranulatedCloud(true);
    DBG("Filepos:" + std::to_string(filePos));
    DBG("duration:" + std::to_string(sampleDuration));
}

void LaGranaAudioProcessor::resetEnvelopes()
{
    //float durationValue = treeState.getRawParameterValue("grain_durations")->load() * FileLoader::getInstance()->getSampleRate() / 1000;
    float durationValue = granulatorModel.getGrainSize() * FileLoader::getInstance()->getSampleRate() / 1000;
    int sampleDuration = (int)durationValue;
    GaussianEnvelope::reset(sampleDuration, FileLoader::getInstance()->getSampleRate(), 0.8f);
    TrapezoidalEnvelope::reset(sampleDuration, FileLoader::getInstance()->getSampleRate(), 0.8f);
    RaisedCosineBellEnvelope::reset(sampleDuration, FileLoader::getInstance()->getSampleRate(), 0.8f);
}

void LaGranaAudioProcessor::play()
{
    if(granulatorModel.getHasGranulatedCloud())
        this->granulator.initialize();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LaGranaAudioProcessor();
}
