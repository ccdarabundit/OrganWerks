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

// Adding in Faust
class lossyPipeEngine;
class MapUI;
using namespace juce;

enum Rank
{
    // Principal
    PRINCIPAL8 = 0,
    PRINCIPAL16,
    PRINCIPAL513,
    PRINCIPAL4,
    PRINCIPAL223,
    PRINCIPAL2,
    // Flutes
    FLUTE8,
    FLUTE16,
    FLUTE513,
    FLUTE4,
    FLUTE223,
    FLUTE2,
    // Violin
    VIOLIN8,
    VIOLIN16,
    VIOLIN513,
    VIOLIN4,
    VIOLIN223,
    VIOLIN2
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
        fDSP->instanceClear();
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
        fDSP->instanceClear();
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
        setGain(level);
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
    lossyPipeEngine* fDSP;
    Rank voiceRank;
    leakySmooth env;
    float tailLength;
    static double principalDelay[65];
    static double principalCoefs[65][5];
    static double fluteDelay[65];
    static double fluteCoefs[65][5];
    static double violinDelay[65];
    static double violinCoefs[65][5];
    // Required functions
    bool setMidi(int midiNoteNumber)
    {
        switch (voiceRank)
        {
            case PRINCIPAL8:
            case FLUTE8:
            case VIOLIN8:
                break;
            case PRINCIPAL4:
            case FLUTE4:
            case VIOLIN4:
                midiNoteNumber += 12;
                break;
            case PRINCIPAL16:
            case FLUTE16:
            case VIOLIN16:
                midiNoteNumber -= 12;
                break;
            case PRINCIPAL513:
            case FLUTE513:
            case VIOLIN513:
                midiNoteNumber += 7;
                break;
            case PRINCIPAL223:
            case FLUTE223:
            case VIOLIN223:
                midiNoteNumber += 19;
                break;
            case PRINCIPAL2:
            case FLUTE2:
            case VIOLIN2:
                midiNoteNumber += 24;
                break;
        }
        
        if (midiNoteNumber < 32)
            return 0;
        if (midiNoteNumber > 96)
            return 0;
        
        fUI->setParamValue("MIDI Note", midiNoteNumber);
        selectPipe(midiNoteNumber);
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
    
    void selectPipe(int midiNoteNumber)
    {
        int mInd = midiNoteNumber - 32;
        float b0, b1, b2, a1, a2, delay;
        if (voiceRank >= PRINCIPAL8 && voiceRank <= PRINCIPAL2)
        {
            b0 = principalCoefs[mInd][0]; b1 = principalCoefs[mInd][1]; b2 = principalCoefs[mInd][2];
            a1 = principalCoefs[mInd][3]; a2 = principalCoefs[mInd][4];
            delay = principalDelay[mInd];
        }
        else if (voiceRank >= FLUTE8 && voiceRank <= FLUTE2)
        {
            b0 = fluteCoefs[mInd][0]; b1 = fluteCoefs[mInd][1]; b2 = fluteCoefs[mInd][2];
            a1 = fluteCoefs[mInd][3]; a2 = fluteCoefs[mInd][4];
            delay = fluteDelay[mInd];
        }
        else if (voiceRank >= VIOLIN8 && voiceRank <= VIOLIN2)
        {
            b0 = violinCoefs[mInd][0]; b1 = violinCoefs[mInd][1]; b2 = violinCoefs[mInd][2];
            a1 = violinCoefs[mInd][3]; a2 = violinCoefs[mInd][4];
            delay = violinDelay[mInd];
        }
        
        fUI->setParamValue("b0", b0);
        fUI->setParamValue("b1", b1);
        fUI->setParamValue("b2", b2);
        fUI->setParamValue("a1", a1);
        fUI->setParamValue("a2", a2);
        fUI->setParamValue("Group Delay", delay);
    }
};
