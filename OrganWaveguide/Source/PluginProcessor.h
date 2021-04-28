/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OrganSound.h"
#include "OrganVoice.h"
#define NUM_RANKS 3
#define NUM_VOICES 12
//==============================================================================
/**
*/
class OrganWaveguideAudioProcessor  : public juce::AudioProcessor
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
    void setStopGain(float newGain, int index);
    void keyOn(int pitch);
    void keyOff(int pitch);
private:
    Synthesiser Organ[NUM_RANKS];
    OrganVoice* myVoice;
    double stopGains[NUM_RANKS];
    double lastSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrganWaveguideAudioProcessor)
};
