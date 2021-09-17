/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Gfdn_pluginAudioProcessorEditor  : public AudioProcessorEditor,
                                         public Slider::Listener,
                                         private Timer
{
public:
    Gfdn_pluginAudioProcessorEditor (Gfdn_pluginAudioProcessor&, AudioProcessorValueTreeState&);
    ~Gfdn_pluginAudioProcessorEditor();
    
    //create this structure
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged (Slider*) override;
    
    enum
    {
        nGroups = 2,
    };

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Gfdn_pluginAudioProcessor& processor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    
    juce::Label dryMixLabel, couplingCoeffLabel, mixingFracLabel, t60lowLabel, t60highLabel, transFreqLabel;
    juce::Slider dryMixSlider, couplingCoeffSlider, mixingFracSlider[nGroups], t60lowSlider[nGroups], t60highSlider[nGroups], transFreqSlider[nGroups];
    std::unique_ptr <SliderAttachment> dryMixAttach, couplingCoeffAttach, mixingFracAttach[nGroups], t60lowAttach[nGroups], t60highAttach[nGroups], transFreqAttach[nGroups];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gfdn_pluginAudioProcessorEditor)
};
