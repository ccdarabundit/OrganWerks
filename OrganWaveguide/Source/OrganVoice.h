/*
  ==============================================================================

    OrganVoice.h
    Created: 27 Apr 2021 5:51:38pm
    Author:  Champ Darabundit

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "OrganSound.h"
#include "lossyPipeEngine.h"
#include "pipeEngine.h"

// Adding in Faust
class dsp;
class MapUI;
using namespace juce;

enum Rank
{
    PRINCIPAL8 = 0,
    PRINCIPAL4,
    PRINCIPAL16
};

// Smoothing factor for gate
class leakySmooth
{
public:
    leakySmooth(){}
    void setSmooth(float sampleRate, float tau)
    {
        l =0.0001;
    }
    float smooth()
    {
        if (env < 0.99 && envOn)
        {
        env += (1 - env)*l;
        }
        else
        {
            env = 1;
            envOn = false;
        }
        return env;
    }
    
    void trigger(bool trig)
    {
        if (trig)
        {
            env = 0;
            envOn = trig;
        }
    }
    
private:
    bool envOn;
    float l;
    float env;
};

class OrganVoice : public SynthesiserVoice
{
public:
    OrganVoice(){
        fDSP = new lossyPipeEngine();
        fDSP->init(getSampleRate());
        fUI = new MapUI();
        fDSP->buildUserInterface(fUI);
        // Set the constant parameters (not varying now)
        fUI->setParamValue("Jet Offset", 0);
        fUI->setParamValue("Mix", 0.75);
        voiceRank = PRINCIPAL8;
        env.setSmooth(getSampleRate(), 0.001);
        
    }
    
    // Constructor for given rank
    OrganVoice(Rank newRank)
    {
        fDSP = new lossyPipeEngine();
        fDSP->init(getSampleRate());
        fUI = new MapUI();
        fDSP->buildUserInterface(fUI);
        // Set the constant parameters (not varying now)
        fUI->setParamValue("Jet Offset", 0);
        fUI->setParamValue("Mix", 0.75);
        voiceRank = newRank;
        env.setSmooth(getSampleRate(), 0.001);
    }
    
    ~OrganVoice(){
        delete fDSP;
        delete fUI;
    }
    bool canPlaySound(SynthesiserSound* sound)
    {
        // Check if can cast to our synth sound class
        return dynamic_cast<OrganSound*>(sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition)
    {
        // std::cout << midiNoteNumber << std::endl;
        level = velocity;
        // std::cout << freq << std::endl;
        gate = true;
        setMidi(midiNoteNumber);
        setGain(level*0.5);
        setGate(1);
        env.trigger(true);
    }
    
    void stopNote(float velocity, bool allowTailOff)
    {
            clearCurrentNote();
        
            gate = false;
            setGate(0);
            env.trigger(false);
        fDSP->instanceClear();
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
                float envVal =env.smooth();
                outputBuffer.addSample(channel, samp, envVal*stopGain*outputs[channel][samp]);
                ++startSample;
            }
            // Delete that channel (risky?)
            delete [] outputs[channel];
        }
        delete [] outputs; // Clear our memory
        
    }
    void setStopGain(float newStopGain){
        stopGain = newStopGain;
    }
private:
    double level;
    double freq;
    float stopGain;
    bool gate;
    // Faust system
    MapUI* fUI;
    dsp* fDSP;
    Rank voiceRank;
    leakySmooth env;
    // Required functions
    void setMidi(int midiNoteNumber)
    {
        if (midiNoteNumber < 32)
            midiNoteNumber = 32;
        if (midiNoteNumber > 96)
            midiNoteNumber = 96;
        
        fUI->setParamValue("MIDI Note", midiNoteNumber);
    }
    
    void setGain(float gain)
    {
        fUI->setParamValue("Gain", gain);
    }

    void setGate(bool gate)
    {
        if(gate){
            fUI->setParamValue("Gate", 1);
        }
        else{
            fUI->setParamValue("Gate", 0);
        }
    }
    
};
