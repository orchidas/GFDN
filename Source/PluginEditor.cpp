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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //set up sliders
    //Dry/Wet mix
    addAndMakeVisible (&dryMixSlider);
    dryMixSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    dryMixSlider.setRange (0,1.0);
    dryMixSlider.setValue(0.5f);
    dryMixSlider.addListener(this);
    dryMixAttach.reset (new SliderAttachment (valueTreeState, "dryMix", dryMixSlider));

    
    addAndMakeVisible (dryMixLabel);
    dryMixLabel.setText ("Dry/Wet mix", juce::dontSendNotification);
    dryMixLabel.attachToComponent (&dryMixSlider, true);
    
    //Mixing angle
    addAndMakeVisible (&mixingFracSlider);
    mixingFracSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mixingFracSlider.setRange (0,1.0);
    mixingFracSlider.addListener (this);
    mixingFracAttach.reset (new SliderAttachment (valueTreeState, "mixingFrac", mixingFracSlider));

    
    addAndMakeVisible (mixingFracLabel);
    mixingFracLabel.setText ("Mixing % ", juce::dontSendNotification);
    mixingFracLabel.attachToComponent (&mixingFracSlider, true);
    
    //T60 at DC
    addAndMakeVisible (&t60lowSlider);
    t60lowSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    t60lowSlider.setRange (0.01, 20.0);
    t60lowSlider.setValue(1.0);
    t60lowSlider.setTextValueSuffix (" s");
    t60lowSlider.addListener (this);
    t60lowAttach.reset (new SliderAttachment (valueTreeState, "t60low", t60lowSlider));

    
    addAndMakeVisible (t60lowLabel);
    t60lowLabel.setText ("Low T60", juce::dontSendNotification);
    t60lowLabel.attachToComponent (&t60lowSlider, true);
    
    //T60 at nyquist
    addAndMakeVisible (t60highSlider);
    t60highSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    t60highSlider.setRange (0.01, 10.0);
    t60highSlider.setValue(0.5);
    t60highSlider.setTextValueSuffix (" s");
    t60highSlider.addListener (this);
    t60highAttach.reset (new SliderAttachment (valueTreeState, "t60high", t60highSlider));

    
    addAndMakeVisible (t60highLabel);
    t60highLabel.setText ("High T60", juce::dontSendNotification);
    t60highLabel.attachToComponent (&t60highSlider, true);
    
    //transition frequency
    addAndMakeVisible (transFreqSlider);
    transFreqSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    transFreqSlider.setRange (100, 10000);
    transFreqSlider.setTextValueSuffix (" Hz");
    transFreqSlider.addListener (this);
    transFreqAttach.reset (new SliderAttachment (valueTreeState, "transFreq", transFreqSlider));

    
    addAndMakeVisible (transFreqLabel);
    transFreqLabel.setText ("Trans. Frequency", juce::dontSendNotification);
    transFreqLabel.attachToComponent (&transFreqSlider, true);
    
    getLookAndFeel().setColour (Slider::rotarySliderFillColourId, Colours::white);
    startTimer(50);

}

Gfdn_pluginAudioProcessorEditor::~Gfdn_pluginAudioProcessorEditor()
{
}

//==============================================================================
void Gfdn_pluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

//    g.setColour (Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void Gfdn_pluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto sliderLeft = 80;
    dryMixSlider.setBounds (sliderLeft, 50, getWidth() - sliderLeft - 10, 50);
    mixingFracSlider.setBounds (sliderLeft, 100, getWidth() - sliderLeft - 10, 50);
    t60lowSlider.setBounds (sliderLeft, 150, getWidth() - sliderLeft - 10, 50);
    t60highSlider.setBounds (sliderLeft, 200, getWidth() - sliderLeft - 10, 50);
    transFreqSlider.setBounds (sliderLeft, 250, getWidth() - sliderLeft - 10, 50);
}

void Gfdn_pluginAudioProcessorEditor::timerCallback()
{
    //getRawParameterValue returns a pointer to a float, alternately use getParameterAsValue
    dryMixSlider.setValue(*valueTreeState.getRawParameterValue("dryMix"));
    mixingFracSlider.setValue(*valueTreeState.getRawParameterValue("mixingFrac"));
    t60lowSlider.setValue(*valueTreeState.getRawParameterValue("t60low"));
    t60highSlider.setValue(*valueTreeState.getRawParameterValue("t60high"));
    transFreqSlider.setValue(*valueTreeState.getRawParameterValue("transFreq"));

    
}

void Gfdn_pluginAudioProcessorEditor::sliderValueChanged (Slider* slider){
    
    if (slider == &dryMixSlider){
        processor.fdn.updateDryMix(dryMixSlider.getValue());
        *(processor.dryMix) = dryMixSlider.getValue();
    }
    else if (slider == &mixingFracSlider){
        processor.fdn.updateMixingMatrix(mixingFracSlider.getValue());
        *(processor.mixingFrac) = mixingFracSlider.getValue();

    }
    else if(slider == &t60lowSlider || slider == &t60highSlider || slider == &transFreqSlider){
        // in a low shelf filter, the t60 of Nyquist cannot exceed that of DC
        if (slider == &t60highSlider)
            t60highSlider.setValue(t60highSlider.getValue() < t60lowSlider.getValue() ? t60highSlider.getValue() : t60lowSlider.getValue()-0.05);
        
        processor.fdn.updateT60Filter(t60lowSlider.getValue(), t60highSlider.getValue(), transFreqSlider.getValue());
    }
    
}
