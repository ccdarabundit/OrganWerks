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
    void setSmooth(float sampleRate, float attackTime, float releaseTime)
    {
        tauAttack = 1.f - std::exp(-1/(attackTime*sampleRate));
        tauRelease = 1.f - std::exp(-1/(attackTime*sampleRate));
    }
    float smooth()
    {
        tau = attack ? tauAttack : tauRelease;
        if (attack)
        {
            env += (1.f - env)*tau;
        }
        else
        {
            env += -env*tau;
        }
        return env;
    }
    
    void trigger(bool trig)
    {
        if (attack && !trig)
        {
            attack = 0;
        }
        if (!attack && trig)
        {
            attack = 1;
        }
    }
    
private:
    bool envOn;
    float tauAttack, tauRelease, tau;
    float env;
    bool attack = 0;
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
        env.setSmooth(getSampleRate(), 0.0001, 1);
        tailLength = 0.f;
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
        env.setSmooth(getSampleRate(), 0.0001, 1);
        fs = getSampleRate();
        oneOverFs = 1.f/fs;
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
        if (setMidi(midiNoteNumber))
        {
        // std::cout << midiNoteNumber << std::endl;
        level = velocity;
        // std::cout << freq << std::endl;
        gate = true;
        freq = std::pow(2.f, 0.08333*(midiNoteNumber - 69.f))*440;
        setGain(level*0.5);
        setGate(1);
        env.trigger(true);
        tailLength = 0;
        }
    }
    
    void stopNote(float velocity, bool allowTailOff)
    {
        // Turn off note and allow to tail off
        
        if (allowTailOff)
        {
            tailLength = 0.1*fs; //10*fs/freq;
            gate = false;
            setGate(0);
            env.trigger(false);
        }
        else
        {
            gate = false;
            env.trigger(false);
            setGate(0);
            fDSP->instanceClear();
            clearCurrentNote();
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
        
        
            
            for(int samp = 0; samp < numSamples; ++samp)
            {
                //*outputBuffer.getWritePointer(channel,samp) = outputs[channel][samp];
                float envVal = env.smooth();
                for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                {
                    outputBuffer.addSample(channel, samp + startSample ,stopGain*outputs[channel][samp]);
                }
                if ( tailLength > 0)
                {
                    tailLength--;
                }
            }
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            // Delete that channel (risky?)
            delete [] outputs[channel];
        }
        delete [] outputs; // Clear our memory
        
        if (tailLength < 0)
        {
            tailLength = 0;
            fDSP->instanceClear();
            clearCurrentNote();
        }
        
    }
    void setStopGain(float newStopGain){
        stopGain = newStopGain;
    }
private:
    double level;
    double freq, fs, oneOverFs;
    float stopGain;
    bool gate;
    // Faust system
    MapUI* fUI;
    dsp* fDSP;
    Rank voiceRank;
    leakySmooth env;
    float tailLength;
    // Required functions
    bool setMidi(int midiNoteNumber)
    {
        switch (voiceRank)
        {
            case PRINCIPAL8:
                break;
            case PRINCIPAL4:
                midiNoteNumber += 12;
                break;
            case PRINCIPAL16:
                midiNoteNumber -= 12;
                break;
        }
        
        if (midiNoteNumber < 32)
            return 0;
        if (midiNoteNumber > 96)
            return 0;
        
        fUI->setParamValue("MIDI Note", midiNoteNumber);
        return 1;
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
