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
    setSize (400, 375);
    
    //set up sliders
    //Dry/Wet mix
    addAndMakeVisible (&dryMixSlider);
    dryMixSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    dryMixSlider.setRange (0,100.0);
    dryMixSlider.setValue(50.0f);
    dryMixSlider.addListener(this);
    dryMixAttach.reset (new SliderAttachment (valueTreeState, "dryMix", dryMixSlider));

    
    addAndMakeVisible (dryMixLabel);
    dryMixLabel.setText ("Dry/Wet mix", juce::dontSendNotification);
    dryMixLabel.attachToComponent (&dryMixSlider, true);
    
    addAndMakeVisible (&couplingCoeffSlider);
    couplingCoeffSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    couplingCoeffSlider.setRange (0,100.0);
    couplingCoeffSlider.setValue(10.0);
    couplingCoeffSlider.addListener(this);
    
    //Dry/Wet mix
    addAndMakeVisible (couplingCoeffLabel);
    couplingCoeffLabel.setText ("Coupling %", juce::dontSendNotification);
    couplingCoeffLabel.attachToComponent (&couplingCoeffSlider, true);
    
    couplingCoeffSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, couplingCoeffSlider.getTextBoxHeight());
    dryMixSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 50, dryMixSlider.getTextBoxHeight());
    
    for(int i = 0 ;i < nGroups; i++){
    
        //Mixing angle
        addAndMakeVisible (&mixingFracSlider[i]);
        mixingFracSlider[i].setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        mixingFracSlider[i].setRange (0.0,100.0f);
        mixingFracSlider[i].setValue (0.0f);
        mixingFracSlider[i].addListener (this);
        mixingFracSlider[i].setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, mixingFracSlider[i].getTextBoxHeight());
        mixingFracAttach[i].reset (new SliderAttachment (valueTreeState, "mixingFrac", mixingFracSlider[i]));


      
        
        //T60 at DC
        addAndMakeVisible (&t60lowSlider[i]);
        t60lowSlider[i].setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        t60lowSlider[i].setRange (0.01, 20.0);
        t60lowSlider[i].setValue(1.0);
        t60lowSlider[i].setTextValueSuffix (" s");
        t60lowSlider[i].addListener (this);
        t60lowSlider[i].setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, t60lowSlider[i].getTextBoxHeight());
        t60lowAttach[i].reset (new SliderAttachment (valueTreeState, "t60low", t60lowSlider[i]));

        
      
        
        //T60 at nyquist
        addAndMakeVisible (t60highSlider[i]);
        t60highSlider[i].setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        t60highSlider[i].setRange (0.01, 10.0);
        t60highSlider[i].setValue(0.5);
        t60highSlider[i].setTextValueSuffix (" s");
        t60highSlider[i].addListener (this);
        t60highSlider[i].setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, t60highSlider[i].getTextBoxHeight());
        t60highAttach[i].reset (new SliderAttachment (valueTreeState, "t60high", t60highSlider[i]));

        
       
        
        //transition frequency
        addAndMakeVisible (transFreqSlider[i]);
        transFreqSlider[i].setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        transFreqSlider[i].setRange (100, 10000);
        transFreqSlider[i].setTextValueSuffix (" Hz");
        transFreqSlider[i].addListener (this);
        transFreqSlider[i].setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, transFreqSlider[i].getTextBoxHeight());
        transFreqAttach[i].reset (new SliderAttachment (valueTreeState, "transFreq", transFreqSlider[i]));

        
       
     }
    
    //other labels
    
    addAndMakeVisible(mixingFracLabel);
    mixingFracLabel.setText ("Mixing % ", juce::dontSendNotification);
    mixingFracLabel.attachToComponent (&mixingFracSlider[0], true);
    
    addAndMakeVisible (t60lowLabel);
    t60lowLabel.setText ("Low T60", juce::dontSendNotification);
    t60lowLabel.attachToComponent (&t60lowSlider[0], true);
    
    addAndMakeVisible (t60highLabel);
    t60highLabel.setText ("High T60", juce::dontSendNotification);
    t60highLabel.attachToComponent (&t60highSlider[0], true);
    
    addAndMakeVisible (transFreqLabel);
    transFreqLabel.setText ("Trans. Frequency", juce::dontSendNotification);
    transFreqLabel.attachToComponent (&transFreqSlider[0], true);
    
    //getLookAndFeel().setColour (Slider::LinearHorizontalSliderFillColourId, Colours::white);
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
    
    for(int i = 0; i < nGroups; i++){
        
        mixingFracSlider[i].setBounds (sliderLeft + i*150, 10, getWidth() - sliderLeft - 180, 50);
        t60lowSlider[i].setBounds (sliderLeft + i*150, 60, getWidth() - sliderLeft - 180, 50);
        t60highSlider[i].setBounds (sliderLeft + i*150, 110, getWidth() - sliderLeft - 180, 50);
        transFreqSlider[i].setBounds (sliderLeft + i*150, 160, getWidth() - sliderLeft - 180, 50);
        //source[i].setBounds(sliderLeft + i*150, 310, getWidth() - sliderLeft - 50, 20);
        //listener[i].setBounds(sliderLeft  + i*150, 340, getWidth() - sliderLeft - 50, 20);
    }
    
    dryMixSlider.setBounds (sliderLeft , 210, getWidth() - sliderLeft - 10, 50);
    couplingCoeffSlider.setBounds (sliderLeft, 260, getWidth() - sliderLeft - 10, 50);
}

void Gfdn_pluginAudioProcessorEditor::timerCallback()
{
    //getRawParameterValue returns a pointer to a float, alternately use getParameterAsValue
    dryMixSlider.setValue(*valueTreeState.getRawParameterValue("dryMix"));
    couplingCoeffSlider.setValue(*valueTreeState.getRawParameterValue("couplingCoeff"));
    
    for(int i = 0; i < nGroups; i++){
        mixingFracSlider[i].setValue(*valueTreeState.getRawParameterValue("mixingFrac" + std::to_string(i)));
        t60lowSlider[i].setValue(*valueTreeState.getRawParameterValue("t60low" + std::to_string(i)));
        t60highSlider[i].setValue(*valueTreeState.getRawParameterValue("t60high" + std::to_string(i)));
        transFreqSlider[i].setValue(*valueTreeState.getRawParameterValue("transFreq" + std::to_string(i)));
    }

    
}

void Gfdn_pluginAudioProcessorEditor::sliderValueChanged (Slider* slider){
    
    if (slider == &dryMixSlider){
        processor.gfdn.updateDryMix(dryMixSlider.getValue()/100.0);
        *(processor.dryMix) = dryMixSlider.getValue();
    }
    else if(slider == &couplingCoeffSlider){
        processor.gfdn.updateCouplingCoeff(couplingCoeffSlider.getValue()/100.0);
        *(processor.couplingCoeff) = couplingCoeffSlider.getValue();
    }
    else{
        for (int i = 0; i < nGroups; i++)
        {
            if (slider == &mixingFracSlider[i]){
                processor.gfdn.updateMixingMatrix(mixingFracSlider[i].getValue()/100.0, i);
                *(processor.mixingFrac[i]) = mixingFracSlider[i].getValue();
            }

    
            else if(slider == &t60lowSlider[i] || slider == &t60highSlider[i] || slider == &transFreqSlider[i]){
                // in a low shelf filter, the t60 of Nyquist cannot exceed that of DC
                if (slider == &t60highSlider[i])
                    t60highSlider[i].setValue(t60highSlider[i].getValue() < t60lowSlider[i].getValue() ? t60highSlider[i].getValue() : t60lowSlider[i].getValue()-0.05);
                
                processor.gfdn.updateT60Filter(t60lowSlider[i].getValue(), t60highSlider[i].getValue(), transFreqSlider[i].getValue(), i);
                *(processor.t60low[i]) = t60lowSlider[i].getValue();
                *(processor.t60high[i]) = t60highSlider[i].getValue();
                *(processor.transFreq[i]) = transFreqSlider[i].getValue();

            }
        }
    }
    
}
