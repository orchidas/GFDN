/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GFDN.h"

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
    void initializeDelayLengths(float sampleRate);

    enum
    {
        nGroups = 2,
    };
    
    juce::AudioProcessorValueTreeState parameters;
    GFDN gfdn;
    
    //Input parameters
    std::atomic<float>* dryMix; // Mix level of original signal;
    std::atomic<float>* couplingCoeff; // Coupling coefficient between rooms;
    std::atomic<float>* beta;   //parameter to control diffraction filter cut-off frequency
    std::atomic<float>* mixingFrac[nGroups];  // amount of mixing (0-1)
    std::atomic<float>* t60low[nGroups];  // T60 of low frequencies
    std::atomic<float>* t60high[nGroups];    // T60 of high frequencies
    std::atomic<float>* transFreq[nGroups];   //transition frequency of shelf filter
    std::atomic<float>* sourcePos;     //where is source placed (I couldn't figure out how to do this with bool)
    std::atomic<float>* listenerPos;   //where is listener placed

private:
    float prevCouplingCoeff = 0.0f;
    float prevBeta = 0.0f;
    float prevMixingFrac[nGroups];
    float prevLowT60[nGroups];
    float prevHighT60[nGroups];
    float prevTransitionFrequency[nGroups];
    float prevSourcePos;
    float prevListenerPos;
    const static int numDelLinesPerGroup = 8;
    

    std::vector<std::vector<float>> inputData;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gfdn_pluginAudioProcessor)
    
    int nDelayLines[nGroups] = {numDelLinesPerGroup,numDelLinesPerGroup};
    int **delayLengths;
    int LR = 25;  //lower range of delay lines in ms (non-overlapping)
    int UR = 50;  //upper range of delay lines in ms
};
