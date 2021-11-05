/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Gfdn_pluginAudioProcessorEditor::Gfdn_pluginAudioProcessorEditor (Gfdn_pluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{   
    //set up sliders
    //Dry/Wet mix
    addAndMakeVisible (&dryMixSlider);
    dryMixSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    dryMixSlider.setRange (0,100.0);
    dryMixSlider.setValue(50.0f);
    dryMixAttach.reset (new SliderAttachment (valueTreeState, "dryMix", dryMixSlider));

    addAndMakeVisible (dryMixLabel);
    dryMixLabel.setText ("Dry/Wet mix", juce::dontSendNotification);
    dryMixLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    dryMixLabel.attachToComponent (&dryMixSlider, true);

    //coupling coefficient
    addAndMakeVisible (&couplingCoeffSlider);
    couplingCoeffSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    couplingCoeffSlider.setRange (0,100.0);
    couplingCoeffSlider.setValue(10.0);
    
    addAndMakeVisible (couplingCoeffLabel);
    couplingCoeffLabel.setText ("Coupling %", juce::dontSendNotification);
    couplingCoeffLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    couplingCoeffLabel.attachToComponent (&couplingCoeffSlider, true);
    
    //diffraction filter cutoff
    addAndMakeVisible (&betaSlider);
    betaSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    betaSlider.setRange (0,100.0);
    betaSlider.setValue(50.0);
    
    addAndMakeVisible (betaLabel);
    betaLabel.setText ("Mix. filt. cutoff", juce::dontSendNotification);
    betaLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    betaLabel.attachToComponent (&betaSlider, true);
    
    couplingCoeffSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, couplingCoeffSlider.getTextBoxHeight());
    betaSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, couplingCoeffSlider.getTextBoxHeight());
    dryMixSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, dryMixSlider.getTextBoxHeight());
    
    
    //other labels
    addAndMakeVisible(mixingFracLabel);
    mixingFracLabel.setText ("Mixing % ", juce::dontSendNotification);
    mixingFracLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    
    addAndMakeVisible (t60lowLabel);
    t60lowLabel.setText ("Low T60", juce::dontSendNotification);
    t60lowLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    
    addAndMakeVisible (t60highLabel);
    t60highLabel.setText ("High T60", juce::dontSendNotification);
    t60highLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    
    addAndMakeVisible (transFreqLabel);
    transFreqLabel.setText ("Trans. Frequency", juce::dontSendNotification);
    transFreqLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    
    addAndMakeVisible(sourceLabel);
    sourceLabel.setText ("Source", juce::dontSendNotification);
    sourceLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    
    addAndMakeVisible(listenerLabel);
    listenerLabel.setText ("Listener", juce::dontSendNotification);
    listenerLabel.setFont(Font ("Times New Roman", 15.0f, Font::plain));

    for (int i = 0; i < nGroups; i++) {
        auto editor = roomEditors.add(std::make_unique<RoomEditor>(valueTreeState, i));
        addAndMakeVisible (editor);
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 400);
}

Gfdn_pluginAudioProcessorEditor::~Gfdn_pluginAudioProcessorEditor()
{
}

//==============================================================================
void Gfdn_pluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont(Font ("Times New Roman", 15.0f, Font::plain));
    g.setColour (Colours::lightgrey);
    g.drawText ("Room 1", 120, 5, 50, 10, true);
    g.drawText ("Room 2", 270, 5, 50, 10, true);
    
    g.setFont (Font ("Times New Roman", 20.0f, Font::bold));
    g.setColour (Colours::lightgrey);
    g.drawText ("GFDN", 320, 300, 100, 50, true);
    g.drawText ("Reverb", 320, 320, 100, 50, true);
}

void Gfdn_pluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto sliderLeft = 100;
    
    mixingFracLabel.setBounds(0, 10, sliderLeft, 50);
    t60lowLabel.setBounds(0, 60, sliderLeft, 50);
    t60highLabel.setBounds(0, 110, sliderLeft, 50);
    transFreqLabel.setBounds(0, 160, sliderLeft, 50);
    sourceLabel.setBounds(0, 210, sliderLeft, 20);
    listenerLabel.setBounds(0, 230, sliderLeft, 20);
    
    int editorWidth = (getWidth() - sliderLeft) / roomEditors.size();
    for (int i = 0; i < nGroups; ++i) {
        roomEditors[i]->setBounds(sliderLeft + i * editorWidth, 10, editorWidth, 240);
    }

    dryMixSlider.setBounds (sliderLeft - 25, 250, getWidth() - sliderLeft - 50, 50);
    couplingCoeffSlider.setBounds (sliderLeft - 25, 300, getWidth() - sliderLeft - 50, 50);
    betaSlider.setBounds (sliderLeft - 25, 350, getWidth() - sliderLeft - 50, 50);

}
