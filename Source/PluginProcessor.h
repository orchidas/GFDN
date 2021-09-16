/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FDN.h"

//==============================================================================
/**
*/
class Gfdn_pluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Gfdn_pluginAudioProcessor();
    ~Gfdn_pluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState parameters;
    FDN fdn;
    
    //Input parameters
    std::atomic<float>* dryMix; // Mix level of original signal;
    std::atomic<float>* mixingFrac;  // amount of mixing (0-1)
    std::atomic<float>* t60low;  // T60 of low frequencies
    std::atomic<float>* t60high;    // T60 of high frequencies
    std::atomic<float>* transFreq;   //transition frequency of shelf filter

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gfdn_pluginAudioProcessor)
    
    int nDelayLines = 8;
};
