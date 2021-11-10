#pragma once

#include "pch.h"
#include "dsp/OrganSound.h"
#include "dsp/OrganVoice.h"
#include "dsp/declicker.h"
#include "gui/organLAF.h"

// Define number of pipes
#define NUM_RANKS 23
#define NUM_VOICES 12


class OrganWaveguide  : public foleys::MagicProcessor, private juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    OrganWaveguide();
    ~OrganWaveguide() override;
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void parameterChanged (const juce::String& param, float value) override;
    
private:
    // Synth defines
    Synthesiser Organ[NUM_RANKS];
    OrganVoice* myVoice;

    // Pipe parameters
    double stopGains[NUM_RANKS];
    bool voiceOn[NUM_RANKS];

    double lastSampleRate;
    float outputGain;

    // PGM
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    foleys::MagicProcessorState magicState { *this };
    declicker<float> gainDeclick;

    // Parameter setting functions
    void setStopGain(float newGain, int index);
    void setEnvelopeAttack(float newAttack);
    void setOutputGain(float newOutputGain);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrganWaveguide)
};
