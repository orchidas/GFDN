/*
  ==============================================================================

    RoomEditor.h
    Created: 26 Sep 2021 3:13:56pm
    Author:  jatin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RoomEditor  : public juce::Component
{
public:
    RoomEditor (AudioProcessorValueTreeState& vts, int roomIdx);
    ~RoomEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider mixingFracSlider, t60lowSlider, t60highSlider, transFreqSlider;
    juce::ToggleButton sourcePos, listenerPos;
    std::unique_ptr <SliderAttachment> mixingFracAttach, t60lowAttach, t60highAttach, transFreqAttach;
    std::unique_ptr <juce::ParameterAttachment> sourcePosAttach, listenerPosAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoomEditor)
};
