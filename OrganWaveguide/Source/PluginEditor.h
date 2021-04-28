/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
using namespace juce;
class OrganWaveguideAudioProcessorEditor  : 
public juce::AudioProcessorEditor
{
public:
    OrganWaveguideAudioProcessorEditor (OrganWaveguideAudioProcessor&);
    ~OrganWaveguideAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Sliders for voices
    Slider stopGainSliders[NUM_VOICES];
    Label stopLabels[NUM_VOICES];
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OrganWaveguideAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrganWaveguideAudioProcessorEditor)
};
