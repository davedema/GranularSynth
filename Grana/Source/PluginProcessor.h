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
    Model* getModel();
    void granulate();
    void resetEnvelopes();
    void play();            // Called from editor (when PLAY button pressed)


private:
    AudioProcessorValueTreeState treeState; // save the current state of the plugin
    Granulator granulator; // processor kernel where all operations on audio occur
    Model granulatorModel; // a model in which we store the audiotreestate values in order to access them easily

    double sampleRate;
    int samplesPerBlock;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LaGranaAudioProcessor)
};
 