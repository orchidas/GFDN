/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "RoomEditor.h"

//==============================================================================
/**
*/
class Gfdn_pluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Gfdn_pluginAudioProcessorEditor (Gfdn_pluginAudioProcessor&, AudioProcessorValueTreeState&);
    ~Gfdn_pluginAudioProcessorEditor();
    
    //create this structure
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    
    enum
    {
        nGroups = 2,
    };

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Gfdn_pluginAudioProcessor& processor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    OwnedArray<RoomEditor> roomEditors;
    
    juce::Label dryMixLabel, couplingCoeffLabel, mixingFracLabel, t60lowLabel, t60highLabel, transFreqLabel, sourceLabel, listenerLabel;
    juce::Slider dryMixSlider, couplingCoeffSlider;
    std::unique_ptr <SliderAttachment> dryMixAttach, couplingCoeffAttach;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gfdn_pluginAudioProcessorEditor)
};
