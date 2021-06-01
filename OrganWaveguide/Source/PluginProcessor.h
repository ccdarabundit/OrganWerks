/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OrganSound.h"
#include "OrganVoice.h"
#include "declicker.h"
#include "GUI/organLAF.h"
#include "BinaryData.h" 
#define NUM_RANKS 18
#define NUM_VOICES 12
#define NUM_FAMS 1
#define FOLEYS_SHOW_GUI_EDITOR_PALLETTE=0

//==============================================================================
/**
*/
class OrganWaveguideAudioProcessor  : public foleys::MagicProcessor, private juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    OrganWaveguideAudioProcessor();
    ~OrganWaveguideAudioProcessor() override;
    
    
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
    void setStopGain(float newGain, int index);
    void setEnvelopeAttack(float newAttack);
    void setOutputGain(float newOutputGain);
    void keyOn(int pitch);
    void keyOff(int pitch);
private:
    Synthesiser Organ[NUM_RANKS];
    OrganVoice* myVoice;
    double stopGains[NUM_RANKS];
    bool voiceOn[NUM_RANKS];
    double lastSampleRate;
    float outputGain;
    // PGM
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    foleys::MagicProcessorState magicState { *this };
    declicker<float> gainDeclick;
    // Listener values?
    static juce::String principalID;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrganWaveguideAudioProcessor)
};
