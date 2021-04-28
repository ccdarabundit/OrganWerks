/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OrganWaveguideAudioProcessor::OrganWaveguideAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
    {
        Organ[nRanks].clearVoices();
        stopGains[nRanks] = 1.0;
    }
        // Add voices
        for (int nVoices = 0; nVoices < NUM_VOICES; nVoices++)
        {
            Organ[0].addVoice(new OrganVoice(PRINCIPAL8));
            Organ[1].addVoice(new OrganVoice(PRINCIPAL4));
            Organ[2].addVoice(new OrganVoice(PRINCIPAL16));
        }
    
    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
    {
        Organ[nRanks].clearSounds();
        Organ[nRanks].addSound(new OrganSound());
    }
        
}

void OrganWaveguideAudioProcessor::setStopGain(float newGain, int index)
{
    stopGains[index] = newGain;
    for (int nVoice = 0; nVoice < NUM_VOICES; nVoice++)
    {
        dynamic_cast<OrganVoice *>(Organ[index].getVoice(nVoice))->setStopGain(newGain);
    }
}

OrganWaveguideAudioProcessor::~OrganWaveguideAudioProcessor()
{
}

//==============================================================================
const juce::String OrganWaveguideAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OrganWaveguideAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OrganWaveguideAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OrganWaveguideAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OrganWaveguideAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OrganWaveguideAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OrganWaveguideAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OrganWaveguideAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OrganWaveguideAudioProcessor::getProgramName (int index)
{
    return {};
}

void OrganWaveguideAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OrganWaveguideAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock); // clear out last keypress
    lastSampleRate = sampleRate; // protection for synthesizer
    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
    {
        Organ[nRanks].setCurrentPlaybackSampleRate(lastSampleRate);
    }
}

void OrganWaveguideAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OrganWaveguideAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void OrganWaveguideAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

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
    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
        Organ[nRanks].renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    float scale = 1.0/NUM_RANKS;
    buffer.applyGain(scale);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool OrganWaveguideAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OrganWaveguideAudioProcessor::createEditor()
{
    return new OrganWaveguideAudioProcessorEditor (*this);
}

//==============================================================================
void OrganWaveguideAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OrganWaveguideAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrganWaveguideAudioProcessor();
}

float midi2Freq(int pitch)
{
    float tmp = pow(2.0, (pitch - 69.0)/12.0);
    return tmp*440.0;
}

void OrganWaveguideAudioProcessor::keyOn(int pitch)
{
    float freq = midi2Freq(pitch);
}

void OrganWaveguideAudioProcessor::keyOff(int pitch)
{
    float freq = midi2Freq(pitch);
}
