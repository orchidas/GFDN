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
    (ParameterID{"dryMix",1}, // parameterID
     "Dry Mix", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     50.0f), // default value)
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"couplingCoeff",1}, // parameterID
     "Coupling Fraction", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     10.0f),    // default value)
    std::make_unique<juce::AudioParameterFloat>
        (ParameterID{"beta", 1},
     "Mix. filt. Cutoff",
     0.0f,
     100.0f,
     50.0f),
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"mixingFrac0", 1},// parameterID
     "Mixing Fraction", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     0.0f),  //default value
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"t60low0",1}, // parameterID
     "Low T60", // parameter name
     0.05f,   // minimum value
     20.0f,   // maximum value
     1.0f), // default value
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"t60high0",1}, // parameterID
     "High T60", // parameter name
     0.05f,   // minimum value
     10.0f,   // maximum value
     0.5f),
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"transFreq0",1}, // parameterID
     "Trans. Frequency", // parameter name
     100.0f,   // minimum value
     10000.0f,   // maximum value
     200.0f),
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"mixingFrac1",1}, // parameterID
     "Mixing Fraction", // parameter name
     0.0f,   // minimum value
     100.0f,   // maximum value
     0.0f),  //default value
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"t60low1",1}, // parameterID
     "Low T60", // parameter name
     0.05f,   // minimum value
     20.0f,   // maximum value
     3.0f), // default value
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"t60high1",1}, // parameterID
     "High T60", // parameter name
     0.05f,   // minimum value
     10.0f,   // maximum value
     1.0f),
    std::make_unique<juce::AudioParameterFloat>
    (ParameterID{"transFreq1",1}, // parameterID
     "Trans. Frequency", // parameter name
     100.0f,   // minimum value
     10000.0f,   // maximum value
     500.0f),
    std::make_unique<juce::AudioParameterInt>
    (ParameterID{"sourcePos",1},
     "Source",
     0, nGroups - 1, 0),
    std::make_unique<juce::AudioParameterInt>
    (ParameterID{"listenerPos",1},
     "Listener",
     0, nGroups - 1, 0),
    })
#endif
{
    //set user defined parameters
    dryMix = parameters.getRawParameterValue("dryMix");
    couplingCoeff = parameters.getRawParameterValue("couplingCoeff");
    beta = parameters.getRawParameterValue("beta");

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

//function to find prime numbers in a range and permute them to find delay line lengths
//==========================
void Gfdn_pluginAudioProcessor::initializeDelayLengths(float sampleRate){
    int count = 0;
    bool prime;
    int N = 0;
    int group;
    int start_prime = (int)(LR * sampleRate/1000.0);
    int end_prime = (int)(UR * sampleRate/1000.0);
    delayLengths = new int*[nGroups];
    
    for(int i = 0; i < nGroups; i++)
        N+= nDelayLines[i];
    int primeNums[N];
    
    //find prime numbers
    for(int i = start_prime; i <= end_prime; i++){
        group = (int)std::round(count/numDelLinesPerGroup);
        if (count >= N)
            break;
        else{
            prime= true;
            for(int k = 2; k < i/2; k++){
                if (i % k == 0){
                    prime = false;
                    break;
                }
            }
            if (prime == true)
                primeNums[count++] = i;
        }
    }
    
    //permutation in place
    std::next_permutation(primeNums, primeNums + N);
    //copy permuted prime numbers into
    count = 0;
    for(int grp = 0; grp < nGroups; grp++){
        delayLengths[grp] = new int[numDelLinesPerGroup];
        for (int i = 0; i < numDelLinesPerGroup; i++){
            delayLengths[grp][i] = primeNums[count++];
            //std::cout << "Delay line length : " << delayLengths[grp][i] << std::endl;
        }
    }
        
}
//==============================================================================
void Gfdn_pluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int numChannels = getMainBusNumInputChannels();
    
    initializeDelayLengths((float)sampleRate);
    gfdn.initialize(nGroups, (float)sampleRate, nDelayLines, delayLengths, numChannels);
    
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
    
    gfdn.updateBeta(*beta/100.0);
    prevBeta = *beta;

    inputData = std::vector<std::vector<float>>(samplesPerBlock, std::vector<float>(numChannels, 0.0f));
    prevSourcePos = 0;
    prevListenerPos = 0;

     //simple benchmark to look at performance...
#if 0
    AudioBuffer<float> impulseBuffer(2, samplesPerBlock);
    impulseBuffer.setSample(0, 0, 1.0f);
    impulseBuffer.setSample(1, 0, 1.0f);

    MidiBuffer midi;

    int hundredSecondsInSamples = int(100.0 * sampleRate);

    auto start = Time::getMillisecondCounterHiRes();
    for (int i = 0; i < hundredSecondsInSamples; i += samplesPerBlock)
    {
        processBlock(impulseBuffer, midi);
        impulseBuffer.clear();
    }

    auto duration = (Time::getMillisecondCounterHiRes() - start) / 1000.0;

    Logger::writeToLog("Processed 100 seconds of audio in " + String(duration) + " seconds");
    Logger::writeToLog("Realtime factor: " + String(100.0 / duration) + "x");

    jassertfalse;
    duration = 0.0;
#endif
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
    
    if(prevBeta != *beta){
        gfdn.updateBeta(*beta/100.0);
        prevBeta = *beta;
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
            //std::cout << "Input :" << inputData[i][chan] << " Output : " << output[chan] << std::endl;
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
