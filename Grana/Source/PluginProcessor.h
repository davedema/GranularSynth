/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Granulator.h"
#include "FileLoader.h"
#include "GrainCloud.h"
#include "Model.h"

//==============================================================================
/**
*/
class LaGranaAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    LaGranaAudioProcessor();
    ~LaGranaAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    AudioProcessorValueTreeState* getValueTreeState();

    void granulate();
    void resetEnvelopes();
    void play();            // Called from editor (when PLAY button pressed)


private:
    std::atomic<float>* grainParameter = nullptr; //atomic allows different threads to operate on the instance
    std::atomic<float>* filePosParameter = nullptr;
    std::atomic<float>* isPlaying = nullptr;

    AudioProcessorValueTreeState treeState; // save the current state of the plugin
    Granulator granulator;
    double sampleRate;

    Model granulatorModel;

    int samplesPerBlock;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LaGranaAudioProcessor)
};
 