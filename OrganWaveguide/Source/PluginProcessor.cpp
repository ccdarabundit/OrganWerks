/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace parameterIDs
    {
        static String attack ("Attack");
    static String outgain ("Output Gain");
    }
namespace principalIDs
    {
    static String eight ( "P8'");
    static String sixt ( "P16'");
    static String four ( "P4'");
    static String twothree ("P223'");
    static String fivethree ("P513'");
    static String two ("P2'");
    }

namespace fluteIDs {
    static String eight ( "F8'");
    static String sixt ( "F16'");
    static String four ( "F4'");
    static String twothree ("F223'");
    static String fivethree ("F513'");
    static String two ("F2'");
}

namespace violinIDs {
    static String eight ( "V8'");
    static String sixt ( "V16'");
    static String four ( "V4'");
    static String twothree ("V223'");
    static String fivethree ("V513'");
    static String two ("V2'");
}
juce::AudioProcessorValueTreeState::ParameterLayout OrganWaveguideAudioProcessor::createParameterLayout()
{
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
    // OTHER
    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("parameters", "Parameters", "|");
    group->addChild(std::make_unique<juce::AudioParameterFloat>(parameterIDs::attack,  "Attack",  juce::NormalisableRange<float>(0.0001f, 0.5f, 0.0001f), 0.05f));
    group->addChild(std::make_unique<juce::AudioParameterFloat>(parameterIDs::outgain,  "Output Gain",  juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    // RANKS
    auto pGroup = std::make_unique<juce::AudioProcessorParameterGroup>("principal", "Principal", "|");
        pGroup->addChild(std::make_unique<juce::AudioParameterFloat>(principalIDs::eight,  "8'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
        pGroup->addChild(std::make_unique<juce::AudioParameterFloat>(principalIDs::sixt,  "16'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
        pGroup->addChild(std::make_unique<juce::AudioParameterFloat>(principalIDs::four,  "4'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
        pGroup->addChild(std::make_unique<juce::AudioParameterFloat>(principalIDs::twothree,  "2-2/3'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
        pGroup->addChild(std::make_unique<juce::AudioParameterFloat>(principalIDs::fivethree,  "5-1/3'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
        pGroup->addChild(std::make_unique<juce::AudioParameterFloat>(principalIDs::two,  "2'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    
    auto fGroup = std::make_unique<juce::AudioProcessorParameterGroup>("flute", "Flute", "|");
    fGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fluteIDs::eight,  "8'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    fGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fluteIDs::sixt,  "16'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    fGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fluteIDs::four,  "4'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    fGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fluteIDs::twothree,  "2-2/3'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    fGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fluteIDs::fivethree,  "5-1/3'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    fGroup->addChild(std::make_unique<juce::AudioParameterFloat>(fluteIDs::two,  "2'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    
    auto vGroup = std::make_unique<juce::AudioProcessorParameterGroup>("violin", "Violin", "|");
    vGroup->addChild(std::make_unique<juce::AudioParameterFloat>(violinIDs::eight,  "8'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    vGroup->addChild(std::make_unique<juce::AudioParameterFloat>(violinIDs::sixt,  "16'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    vGroup->addChild(std::make_unique<juce::AudioParameterFloat>(violinIDs::four,  "4'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    vGroup->addChild(std::make_unique<juce::AudioParameterFloat>(violinIDs::twothree,  "2-2/3'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    vGroup->addChild(std::make_unique<juce::AudioParameterFloat>(violinIDs::fivethree,  "5-1/3'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    vGroup->addChild(std::make_unique<juce::AudioParameterFloat>(violinIDs::two,  "2'",  juce::NormalisableRange<float>(0.000f, 1.0f, 0.01f), 0.0f));
    
    layout.add (std::move(group), std::move(pGroup), std::move(fGroup), std::move(vGroup));
                        
    return layout;
}
//==============================================================================
OrganWaveguideAudioProcessor::OrganWaveguideAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : foleys::MagicProcessor( BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
  treeState (*this,
               /* undo mgr */ nullptr,
               ProjectInfo::projectName,
               createParameterLayout())
{
    outputGain = 0.5;
    
    
    treeState.addParameterListener(principalIDs::eight, this);
    treeState.addParameterListener(principalIDs::sixt, this);
    treeState.addParameterListener(principalIDs::four, this);
    treeState.addParameterListener(principalIDs::twothree, this);
    treeState.addParameterListener(principalIDs::fivethree, this);
    treeState.addParameterListener(principalIDs::two, this);
    
    treeState.addParameterListener(fluteIDs::eight, this);
    treeState.addParameterListener(fluteIDs::sixt, this);
    treeState.addParameterListener(fluteIDs::four, this);
    treeState.addParameterListener(fluteIDs::twothree, this);
    treeState.addParameterListener(fluteIDs::fivethree, this);
    treeState.addParameterListener(fluteIDs::two, this);
    
    treeState.addParameterListener(violinIDs::eight, this);
    treeState.addParameterListener(violinIDs::sixt, this);
    treeState.addParameterListener(violinIDs::four, this);
    treeState.addParameterListener(violinIDs::twothree, this);
    treeState.addParameterListener(violinIDs::fivethree, this);
    treeState.addParameterListener(violinIDs::two, this);
    treeState.addParameterListener(parameterIDs::attack, this);
    treeState.addParameterListener(parameterIDs::outgain, this);
    
    magicState.setGuiValueTree (BinaryData::OrganGUI_xml, BinaryData::OrganGUI_xmlSize);
    // Organ Initialization
    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
    {
        Organ[nRanks].clearVoices();
        stopGains[nRanks] = 0.f;
    }
        // Add voices
        for (int nVoices = 0; nVoices < NUM_VOICES; nVoices++)
        {
            Organ[PRINCIPAL8].addVoice(new OrganVoice(PRINCIPAL8));
            Organ[PRINCIPAL4].addVoice(new OrganVoice(PRINCIPAL4));
            Organ[PRINCIPAL16].addVoice(new OrganVoice(PRINCIPAL16));
            Organ[PRINCIPAL223].addVoice(new OrganVoice(PRINCIPAL223));
            Organ[PRINCIPAL513].addVoice(new OrganVoice(PRINCIPAL513));
            Organ[PRINCIPAL2].addVoice(new OrganVoice(PRINCIPAL2));
            // FLUTES
            Organ[FLUTE8].addVoice(new OrganVoice(FLUTE8));
            Organ[FLUTE4].addVoice(new OrganVoice(FLUTE4));
            Organ[FLUTE16].addVoice(new OrganVoice(FLUTE16));
            Organ[FLUTE223].addVoice(new OrganVoice(FLUTE223));
            Organ[FLUTE513].addVoice(new OrganVoice(FLUTE513));
            Organ[FLUTE2].addVoice(new OrganVoice(FLUTE2));
            // VIOLINS
            Organ[VIOLIN8].addVoice(new OrganVoice(VIOLIN8));
            Organ[VIOLIN4].addVoice(new OrganVoice(VIOLIN4));
            Organ[VIOLIN16].addVoice(new OrganVoice(VIOLIN16));
            Organ[VIOLIN223].addVoice(new OrganVoice(VIOLIN223));
            Organ[VIOLIN513].addVoice(new OrganVoice(VIOLIN513));
            Organ[VIOLIN2].addVoice(new OrganVoice(VIOLIN2));
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
    gainDeclick.initSampleRate(sampleRate);
    gainDeclick.setTarget(outputGain);
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
    
    // MAGIC GUI: send midi messages to the keyboard state and MidiLearn
    magicState.processMidiBuffer (midiMessages, buffer.getNumSamples(), true);
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
    {
        Organ[nRanks].renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }
    
    float scale = 1.0/NUM_RANKS;
    buffer.applyGain(scale);
    outputGain = gainDeclick.declick();
    buffer.applyGain(outputGain);
}

//==============================================================================
bool OrganWaveguideAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OrganWaveguideAudioProcessor::createEditor()
{
    auto builder = std::make_unique<foleys::MagicGUIBuilder>(magicState);
    builder->registerJUCEFactories();
    builder->registerLookAndFeel("OrganLAF", std::make_unique<organLAF>());
    auto editor = new foleys::MagicPluginEditor (magicState, std::move (builder));
    return editor;
}

//==============================================================================
void OrganWaveguideAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    magicState.getStateInformation(destData);
}

void OrganWaveguideAudioProcessor::parameterChanged (const juce::String& param, float value) {
    if (param == principalIDs::eight)
        setStopGain(value, PRINCIPAL8);
    else if (param == principalIDs::four)
        setStopGain(value, PRINCIPAL4);
    else if (param == principalIDs::sixt)
        setStopGain(value, PRINCIPAL16);
    else if (param == principalIDs::twothree)
        setStopGain(value, PRINCIPAL223);
    else if (param == principalIDs::fivethree)
        setStopGain(value, PRINCIPAL513);
    else if (param == principalIDs::two)
        setStopGain(value, PRINCIPAL2);
    
    if (param == fluteIDs::eight)
        setStopGain(value, FLUTE8);
    else if (param == fluteIDs::four)
        setStopGain(value, FLUTE4);
    else if (param == fluteIDs::sixt)
        setStopGain(value, FLUTE16);
    else if (param == fluteIDs::twothree)
        setStopGain(value, FLUTE223);
    else if (param == fluteIDs::fivethree)
        setStopGain(value, FLUTE513);
    else if (param == fluteIDs::two)
        setStopGain(value, FLUTE2);
    
    if (param == violinIDs::eight)
        setStopGain(value, VIOLIN8);
    else if (param == violinIDs::four)
        setStopGain(value, VIOLIN4);
    else if (param == violinIDs::sixt)
        setStopGain(value, VIOLIN16);
    else if (param == violinIDs::twothree)
        setStopGain(value, VIOLIN223);
    else if (param == violinIDs::fivethree)
        setStopGain(value, VIOLIN513);
    else if (param == violinIDs::two)
        setStopGain(value, VIOLIN2);
    
    if (param == parameterIDs::attack)
        setEnvelopeAttack(value);
    else if (param == parameterIDs::outgain)
        setOutputGain(value);
}
void OrganWaveguideAudioProcessor::setEnvelopeAttack(float newAttack)
{
    for (int nRank = 0; nRank < NUM_RANKS; nRank++){
    for (int nVoice = 0; nVoice < NUM_VOICES; nVoice++)
    {
        dynamic_cast<OrganVoice *>(Organ[nRank].getVoice(nVoice))->setAttack(newAttack);
    }
    }
}

void OrganWaveguideAudioProcessor::setOutputGain(float newOutputGain)
{
    gainDeclick.setTarget(newOutputGain*(0.25*NUM_RANKS));
}
void OrganWaveguideAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    magicState.setStateInformation(data,sizeInBytes,getActiveEditor());
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
