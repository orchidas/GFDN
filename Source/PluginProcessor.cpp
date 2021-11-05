/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
Gfdn_pluginAudioProcessor::Gfdn_pluginAudioProcessor() 
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(*this, nullptr, juce::Identifier ("GFDNPlugin"),{
    std::make_unique<juce::AudioParameterFloat>
    ("dryMix", // parameterID
     "Dry Mix", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     50.0f), // default value)
    std::make_unique<juce::AudioParameterFloat>
    ("couplingCoeff", // parameterID
     "Coupling Fraction", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     10.0f), // default value)
    std::make_unique<juce::AudioParameterFloat>
    ("mixingFrac0", // parameterID
     "Mixing Fraction", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     0.0f),  //default value
    std::make_unique<juce::AudioParameterFloat>
    ("t60low0", // parameterID
     "Low T60", // parameter name
     0.05f,   // minimum value
     20.0f,   // maximum value
     1.0f), // default value
    std::make_unique<juce::AudioParameterFloat>
    ("t60high0", // parameterID
     "High T60", // parameter name
     0.05f,   // minimum value
     10.0f,   // maximum value
     0.5f),
    std::make_unique<juce::AudioParameterFloat>
    ("transFreq0", // parameterID
     "Trans. Frequency", // parameter name
     100.0f,   // minimum value
     10000.0f,   // maximum value
     200.0f),
    std::make_unique<juce::AudioParameterFloat>
    ("mixingFrac1", // parameterID
     "Mixing Fraction", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     0.0f),  //default value
    std::make_unique<juce::AudioParameterFloat>
    ("t60low1", // parameterID
     "Low T60", // parameter name
     0.05f,   // minimum value
     20.0f,   // maximum value
     3.0f), // default value
    std::make_unique<juce::AudioParameterFloat>
    ("t60high1", // parameterID
     "High T60", // parameter name
     0.05f,   // minimum value
     10.0f,   // maximum value
     1.0f),
    std::make_unique<juce::AudioParameterFloat>
    ("transFreq1", // parameterID
     "Trans. Frequency", // parameter name
     100.0f,   // minimum value
     10000.0f,   // maximum value
     500.0f),
    std::make_unique<juce::AudioParameterInt>
    ("sourcePos",
     "Source",
     0, nGroups - 1, 0),
    std::make_unique<juce::AudioParameterInt>
    ("listenerPos",
     "Listener",
     0, nGroups - 1, 0),
    })
#endif
{
    //set user defined parameters
    dryMix = parameters.getRawParameterValue("dryMix");
    couplingCoeff = parameters.getRawParameterValue("couplingCoeff");

    for( int i = 0; i < nGroups; i++){
        mixingFrac[i] = parameters.getRawParameterValue("mixingFrac" + std::to_string(i));
        t60low[i] = parameters.getRawParameterValue("t60low" + std::to_string(i));
        t60high[i] = parameters.getRawParameterValue("t60high" + std::to_string(i));
        transFreq[i] = parameters.getRawParameterValue("transFreq" + std::to_string(i));
    }

    sourcePos = parameters.getRawParameterValue("sourcePos");
    listenerPos = parameters.getRawParameterValue("listenerPos");
}

Gfdn_pluginAudioProcessor::~Gfdn_pluginAudioProcessor()
{
}

//==============================================================================
const String Gfdn_pluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Gfdn_pluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Gfdn_pluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Gfdn_pluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Gfdn_pluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Gfdn_pluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Gfdn_pluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Gfdn_pluginAudioProcessor::setCurrentProgram (int index)
{
}

const String Gfdn_pluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void Gfdn_pluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Gfdn_pluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int numChannels = getMainBusNumInputChannels();
    gfdn.initialize(nGroups, (float)sampleRate, nDelayLines, LR, UR, numChannels);
    
    for(int i = 0; i < nGroups; i++){
        gfdn.updateMixingMatrix(*mixingFrac[i]/100.0, i);
        prevMixingFrac[i] = *mixingFrac[i];
        
        prevLowT60[i] = *t60low[i];
        prevHighT60[i] = *t60high[i];
        prevTransitionFrequency[i] = *transFreq[i];
        gfdn.updateT60Filter(*t60low[i], *t60high[i], *transFreq[i], i);
    }
    
    gfdn.updateDryMix(*dryMix/100.0);

    gfdn.updateCouplingCoeff(*couplingCoeff/100.0);
    prevCouplingCoeff = *couplingCoeff;

    inputData = std::vector<std::vector<float>>(samplesPerBlock, std::vector<float>(numChannels, 0.0f));
    prevSourcePos = 0;
    prevListenerPos = 0;
}

void Gfdn_pluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Gfdn_pluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Gfdn_pluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    // update parameters
    for (int i = 0; i < nGroups; i++) {
        if (prevMixingFrac[i] != *mixingFrac[i]) {
            gfdn.updateMixingMatrix(*mixingFrac[i]/100.0, i);
            prevMixingFrac[i] = *mixingFrac[i];
        }
        if (prevLowT60[i] != *t60low[i] || prevHighT60[i] != *t60high[i] || prevTransitionFrequency[i] != *transFreq[i]){
            gfdn.updateT60Filter (*t60low[i], *t60high[i], *transFreq[i], i);
            prevLowT60[i] = *t60low[i];
            prevHighT60[i] = *t60high[i];
            prevTransitionFrequency[i] = *transFreq[i];
        }
    }

    if (prevSourcePos != *sourcePos){
        gfdn.updateSourceRoom ((int) *sourcePos);
        prevSourcePos = *sourcePos;
    }
    if (prevListenerPos != *listenerPos){
        gfdn.updateListenerRoom ((int) *listenerPos);
        prevListenerPos = *listenerPos;
    }

    gfdn.updateDryMix (*dryMix/100.0);

    if (prevCouplingCoeff != *couplingCoeff) {
        gfdn.updateCouplingCoeff(*couplingCoeff/100.0);
        prevCouplingCoeff = *couplingCoeff;
    }

    //How many channels?
    const int numChannels = buffer.getNumChannels();
    // How many samples in the buffer for this block?
    const int numSamples = buffer.getNumSamples();
    
    float *output;
    
    // read input data into multidimensional array
    for(int chan = 0; chan < numChannels; chan++){
        const float* channelInData = buffer.getReadPointer(chan, 0);

        for (int i = 0; i < numSamples; i++){
            inputData[i][chan] = channelInData[i];
        }
    }
        
    for (int i = 0; i < numSamples; i++){
        output = gfdn.processSample(inputData[i].data(), numChannels);
        
        for(int chan = 0; chan < numChannels; chan++){
            buffer.setSample(chan, i, output[chan]);
        }
    }
    
}

//==============================================================================
bool Gfdn_pluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Gfdn_pluginAudioProcessor::createEditor()
{
    return new Gfdn_pluginAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Gfdn_pluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Gfdn_pluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Gfdn_pluginAudioProcessor();
}
