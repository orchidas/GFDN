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
     1.0f,   // maximum value
     0.5f), // default value)
    std::make_unique<juce::AudioParameterFloat>
    ("mixingFrac", // parameterID
     "Mixing Fraction", // parameter name
     0.0f,   // minimum value
     1.0f,   // maximum value
     0.0f),  //default value
    std::make_unique<juce::AudioParameterFloat>
    ("t60low", // parameterID
     "Low T60", // parameter name
     0.05f,   // minimum value
     20.0f,   // maximum value
     1.0f), // default value
    std::make_unique<juce::AudioParameterFloat>
    ("t60high", // parameterID
     "High T60", // parameter name
     0.05f,   // minimum value
     10.0f,   // maximum value
     0.5f),
    std::make_unique<juce::AudioParameterFloat>
    ("transFreq", // parameterID
     "Trans. Frequency", // parameter name
     100.0f,   // minimum value
     10000.0f,   // maximum value
     200.0f)
    })
#endif
{
    //set user defined parameters
    dryMix = parameters.getRawParameterValue("dryMix");
    mixingFrac = parameters.getRawParameterValue("mixingFrac");
    t60low = parameters.getRawParameterValue("t60low");
    t60high = parameters.getRawParameterValue("t60high");
    transFreq = parameters.getRawParameterValue("transFreq");
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
    fdn.initialize((float)sampleRate, nDelayLines);
    fdn.updateDryMix(*dryMix);
    fdn.updateMixingMatrix(*mixingFrac);
    fdn.updateT60Filter(*t60low, *t60high, *transFreq);

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
    /*ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = buffer->getTotalNumInputChannels();
    auto totalNumOutputChannels = buffer->getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }*/
    
    //How many input channels?
    const int numInputChannels = buffer.getNumChannels();
    // How many samples in the buffer for this block?
    const int numSamples = buffer.getNumSamples();
    
    
    //for (int channel = 0; channel < numInputChannels; ++channel)
    //{
    int channel = 0;
        const float* channelInData = buffer.getReadPointer(channel, 0);
        for (int i = 0; i < numSamples; i++){
            buffer.setSample(channel, i, fdn.processSample(channelInData[i]));
        }
    //}
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