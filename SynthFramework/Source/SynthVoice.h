/*
  ==============================================================================

    SynthVoice.h
    Created: 27 Apr 2021 9:41:38am
    Author:  Champ Darabundit

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"
#include "FaustSynth.h"

// Adding in Faust
class dsp;
class MapUI;
using namespace juce;
class SynthVoice : public SynthesiserVoice
{
public:
    SynthVoice(){
        fDSP = new mydsp();
        fDSP->init(getSampleRate());
        fUI = new MapUI();
        fDSP->buildUserInterface(fUI);
    }
    ~SynthVoice(){
        delete fDSP;
        delete fUI;
    }
    bool canPlaySound(SynthesiserSound* sound)
    {
        // Check if can cast to our synth sound class
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition)
    {
        // Convert MIDI to Hertz
        freq = std::pow(2.0, (midiNoteNumber - 69.0)/12.0)*440.0;
        // std::cout << midiNoteNumber << std::endl;
        level = velocity;
        // std::cout << freq << std::endl;
        gate = true;
        setFreq(freq);
        setGain(level);
        setGate(1);
    }
    
    void stopNote(float velocity, bool allowTailOff)
    {
        if (velocity == 0)
        {
            clearCurrentNote();
            gate = false;
            setGate(0);
        }
    }
    
    void pitchWheelMoved(int newPitchWheelValue)
    {
        level = 0;
    }
    
    void controllerMoved(int controllerNumber, int newControllerValue)
    {
        
    }
    
    void renderNextBlock (AudioBuffer<float> & outputBuffer, int startSample, int numSamples)
    {
        // Allocate temporary output for faust
        int nChannels = outputBuffer.getNumChannels();
        float **outputs = new float*[nChannels];
        
        for (int channel = 0; channel < nChannels; ++channel){
            outputs[channel] = new float[numSamples];
        }
        
        //std::cout << gate << std::endl;
        fDSP->compute(numSamples, NULL, outputs);
        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            
            for(int samp = 0; samp < numSamples; ++samp)
            {
                //*outputBuffer.getWritePointer(channel,samp) = outputs[channel][samp];
                outputBuffer.addSample(channel, samp, outputs[channel][samp]);
                ++startSample;
            }
            // Delete that channel (risky?)
            delete [] outputs[channel];
        }
        delete [] outputs; // Clear our memory
        
    }
    
private:
    double level;
    double freq;
    bool gate;
    // Faust system
    MapUI* fUI;
    dsp* fDSP;
    // Required functions
    void setFreq(float freq)
    {
        fUI->setParamValue("freq", freq);
    }
    
    void setGain(float gain)
    {
        fUI->setParamValue("gain", gain);
    }
    
    void setCutoff(float cutoff)
    {
        fUI->setParamValue("cutoff", cutoff);
    }
    
    void setGate(bool gate)
    {
        if(gate){
            fUI->setParamValue("gate", 1);
        }
        else{
            fUI->setParamValue("gate", 0);
        }
    }
    
};
