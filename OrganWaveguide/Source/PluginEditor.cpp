/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
OrganWaveguideAudioProcessorEditor::OrganWaveguideAudioProcessorEditor (OrganWaveguideAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);

    for (int nRanks = 0; nRanks < NUM_RANKS; nRanks++)
    {
        addAndMakeVisible(stopGainSliders[nRanks]);
        stopGainSliders[nRanks].setRange(0.0, 1.0);
        stopGainSliders[nRanks].setValue(1.0);
        addAndMakeVisible(stopLabels[nRanks]);
        stopLabels[nRanks].attachToComponent(&stopGainSliders[nRanks], true);
    }
    stopGainSliders[0].onValueChange = [this] {
        audioProcessor.setStopGain(stopGainSliders[0].getValue(), 0);};
    stopGainSliders[1].onValueChange = [this] {
        audioProcessor.setStopGain(stopGainSliders[1].getValue(), 1);};
    stopGainSliders[2].onValueChange = [this] {
        audioProcessor.setStopGain(stopGainSliders[2].getValue(), 2);};
    // Add labels manually
    stopLabels[0].setText("8'", dontSendNotification);
    stopLabels[1].setText("4'", dontSendNotification);
    stopLabels[2].setText("16'", dontSendNotification);
}


OrganWaveguideAudioProcessorEditor::~OrganWaveguideAudioProcessorEditor()
{
}

//==============================================================================
void OrganWaveguideAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OrganWaveguideAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int sliderLeft = 80;
    stopGainSliders[0].setBounds(sliderLeft, 0, getWidth() - sliderLeft - 20, 20);
    stopGainSliders[1].setBounds(sliderLeft, 20, getWidth() - sliderLeft - 20, 20);
    stopGainSliders[2].setBounds(sliderLeft, 50, getWidth() - sliderLeft - 20, 20);
}
