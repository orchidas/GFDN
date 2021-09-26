/*
  ==============================================================================

    RoomEditor.cpp
    Created: 26 Sep 2021 3:13:56pm
    Author:  jatin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RoomEditor.h"

//==============================================================================
RoomEditor::RoomEditor (AudioProcessorValueTreeState& vts, int roomIdx)
{
    //Mixing angle
    addAndMakeVisible(&mixingFracSlider);
    mixingFracSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    mixingFracSlider.setRange(0.0, 100.0f);
    mixingFracSlider.setValue(0.0f);
    mixingFracSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 40, mixingFracSlider.getTextBoxHeight());
    mixingFracAttach = std::make_unique<SliderAttachment> (vts, "mixingFrac" + std::to_string (roomIdx), mixingFracSlider);

    //T60 at DC
    addAndMakeVisible(&t60lowSlider);
    t60lowSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    t60lowSlider.setRange(0.01, 20.0);
    t60lowSlider.setValue(1.0);
    t60lowSlider.setTextValueSuffix(" s");
    t60lowSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 40, t60lowSlider.getTextBoxHeight());
    t60lowAttach = std::make_unique<SliderAttachment> (vts, "t60low" + std::to_string (roomIdx), t60lowSlider);

    //T60 at nyquist
    addAndMakeVisible(t60highSlider);
    t60highSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    t60highSlider.setRange(0.01, 10.0);
    t60highSlider.setValue(0.5);
    t60highSlider.setTextValueSuffix(" s");
    t60highSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 40, t60highSlider.getTextBoxHeight());
    t60highAttach = std::make_unique<SliderAttachment> (vts, "t60high" + std::to_string (roomIdx), t60highSlider);

    //transition frequency
    addAndMakeVisible(transFreqSlider);
    transFreqSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    transFreqSlider.setRange(100, 10000);
    transFreqSlider.setTextValueSuffix(" Hz");
    transFreqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 40, transFreqSlider.getTextBoxHeight());
    transFreqAttach = std::make_unique<SliderAttachment> (vts, "transFreq" + std::to_string (roomIdx), transFreqSlider);

    //source and listener positions
    addAndMakeVisible(sourcePos);
    sourcePosAttach = std::make_unique<ParameterAttachment>(*vts.getParameter("sourcePos"), [=] (float value) {
        bool isSelected = value == roomIdx;
        sourcePos.setToggleState(isSelected, sendNotificationSync);
    });
    sourcePos.onClick = [=] {
        if (sourcePos.getToggleState())
            sourcePosAttach->setValueAsCompleteGesture((float) roomIdx);
    };
    sourcePosAttach->sendInitialUpdate();

    addAndMakeVisible(listenerPos);
    listenerPosAttach = std::make_unique<ParameterAttachment>(*vts.getParameter("listenerPos"), [=](float value) {
        bool isSelected = value == roomIdx;
        listenerPos.setToggleState(isSelected, sendNotificationSync);
    });
    listenerPos.onClick = [=] {
        if (listenerPos.getToggleState())
            listenerPosAttach->setValueAsCompleteGesture((float)roomIdx);
    };
    listenerPosAttach->sendInitialUpdate();
}

RoomEditor::~RoomEditor()
{
}

void RoomEditor::paint (juce::Graphics& g)
{
}

void RoomEditor::resized()
{
    auto b = getLocalBounds();
    
    mixingFracSlider.setBounds (b.removeFromTop (50));
    t60lowSlider.setBounds (b.removeFromTop (50));
    t60highSlider.setBounds (b.removeFromTop (50));
    transFreqSlider.setBounds (b.removeFromTop (50));
    sourcePos.setBounds (b.removeFromTop (20));
    listenerPos.setBounds (b.removeFromTop (20));
}
