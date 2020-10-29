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
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    // constructors
    treeState(*this, nullptr, Identifier("CURRENT_STATE"),
        {
        std::make_unique<AudioParameterFloat>("filepos", "Filepos", 0, 100, 0.0f), // id, name, min,max, initial value
        std::make_unique< AudioParameterFloat>("Section Size", "Section Size", 0, 100, 50),
        std::make_unique<AudioParameterBool>("isPlaying", "isPlaying", false),
        std::make_unique<AudioParameterFloat>("envIndex", "envIndex", 1, 3, 1), // 1 gaussian, 2raised, 3 trapezoidal
        std::make_unique<AudioParameterFloat>("envWidth","envWidth", 0, 10000, 0.5),
        std::make_unique< AudioParameterFloat>("Density", "Density", GRAIN_DENSITY_MIN, GRAIN_DENSITY_MAX, 25.0f),
        std::make_unique<AudioParameterFloat>("Grain Size", "Grain Size", GRAIN_MIN, GRAIN_MAX, 25.0f), 
        std::make_unique< AudioParameterFloat>("Speed", "Speed", -2, 2, 1)
})
#endif
{ 
    treeState.addParameterListener("filepos", &granulatorModel);
    treeState.addParameterListener("Section Size", &granulatorModel);
    treeState.addParameterListener("isPlaying", &granulatorModel);
    treeState.addParameterListener("Density", &granulatorModel);
    treeState.addParameterListener("Grain Size", &granulatorModel);
    treeState.addParameterListener("Speed", &granulatorModel);
    treeState.addParameterListener("envIndex", &granulatorModel);
    treeState.addParameterListener("envWidth", &granulatorModel);
    this->granulator.setModel(&granulatorModel);
}

LaGranaAudioProcessor::~LaGranaAudioProcessor()
{
    FileLoader::resetInstance();
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
   
    return false;
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
    this->samplesPerBlock = samplesPerBlock;
    FileLoader::getInstance()->setHostRate(sampleRate);
    granulator.setProcessorSampleRate(sampleRate);
}

void LaGranaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LaGranaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return true;
}
#endif

void LaGranaAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    int currentBufferLength = buffer.getNumSamples();
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, currentBufferLength);

    if (granulatorModel.getHasLoadedFile() && !granulatorModel.getIsPlaying()) {
        granulator.process(buffer, buffer.getNumSamples());
        for (int i = 0; i < buffer.getNumSamples(); i++) // cycle through samples and fill the fft input (might be different from buffer sizes)
        {
            //fill extractor input 
        }
    }
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

Model* LaGranaAudioProcessor::getModel()
{
    return &granulatorModel;
}

void LaGranaAudioProcessor::play()
{
    if (granulatorModel.getHasLoadedFile()) {
        this->granulator.initialize(FileLoader::getInstance()->getAudioBuffer()->getNumSamples());  //file length needed for scheduling grains
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LaGranaAudioProcessor();
}
