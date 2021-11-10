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
#include "declicker.h"

// Adding in Faust
namespace Faust
{
    #include "faust_src/gen/flueEngine.h"
    #include "faust_src/gen/gedecktEngine.h"
    class dsp;
    class flueEngine;
    class gedecktEngine;
    class MapUI;
}


using namespace juce;

// Our available ranks
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
    VIOLIN2,
    // Gedeckt
    GEDECKT8,
    GEDECKT16,
    GEDECKT513,
    GEDECKT4,
    GEDECKT223
};

class OrganVoice : public SynthesiserVoice
{
public:
    OrganVoice(){
        // Build Faust
        fDSP = new Faust::flueEngine();
        fDSP->init(getSampleRate());
        fUI = new Faust::MapUI();
        fDSP->buildUserInterface(fUI);
        // Set constant UIs
        fUI->setParamValue("Jet Offset", 0);
        fUI->setParamValue("Mix", 0.75);
        // Default constructor is a 8' Principal
        voiceRank = PRINCIPAL8;
        fs = getSampleRate();
        oneOverFs = 1.f/fs;
        fDSP->instanceClear();
        stopGain.initSampleRate(getSampleRate());
    }
    
    // Constructor for given rank
    OrganVoice(Rank newRank)
    {
        // Build Faust
        if (newRank >= GEDECKT8 && newRank <= GEDECKT223)
        {
            fDSP = new Faust::gedecktEngine();
        }
        else
        {
            fDSP = new Faust::flueEngine();
        }
        voiceRank = newRank;
        fDSP->init(getSampleRate());
        fUI = new Faust::MapUI();
        fDSP->buildUserInterface(fUI);
        // Set constant UIs
        fUI->setParamValue("Jet Offset", 0);
        fs = getSampleRate();
        oneOverFs = 1.f/fs;
        fDSP->instanceClear();
        stopGain.initSampleRate(getSampleRate());
    }
    
    ~OrganVoice()
    {
        delete fDSP;    // Dump Faust
        delete fUI;
    }

    bool canPlaySound(SynthesiserSound* sound)
    {
        // Check if can cast to our synth sound class
        return dynamic_cast<OrganSound*>(sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition)
    {
        // Check we have a viable midi number
        // If not no sound
        if (setMidi(midiNoteNumber))
        {
            // Set gain based on velocity
            setGain(velocity);
            setGate(1);
            // Clear tail length
            tailLength = 0;
        }
    }
    
    void stopNote(float velocity, bool allowTailOff)
    {
        // Turn off note and allow to tail off
        if (allowTailOff)
        {
            tailLength = 0.05*fs; // static tail length 100ms
            setGate(0);
        }
        else
        {
            setGate(0);
            fDSP->instanceClear();
            clearCurrentNote();
        }
        
    }
    
    void pitchWheelMoved(int newPitchWheelValue)
    {

    }
    
    void controllerMoved(int controllerNumber, int newControllerValue)
    {
  
    }
    
    void renderNextBlock (AudioBuffer<float> & outputBuffer, int startSample, int numSamples)
    {
        if (offFlag) {} 
        else
        {
            // Allocate temporary output for Faust
            int nChannels = outputBuffer.getNumChannels();
            float **outputs = new float*[nChannels];
            
            for (int channel = 0; channel < nChannels; ++channel){
                outputs[channel] = new float[numSamples];
            }
            
            // Call Faust 
            fDSP->compute(numSamples, NULL, outputs);

            // Update stopGain and accumulate Faust output to buffer
            float gain = stopGain.declick();
            outputBuffer.addFrom(0, startSample, &outputs[0][0], numSamples, gain);
            outputBuffer.addFrom(1, startSample, &outputs[1][0], numSamples, gain);

            // Sample decrementing for tailing off
            if ( tailLength > 0)
            {
                    tailLength-= numSamples;
            }

            // Reset if exceeded tailLength
            if (tailLength < 0)
            {
                tailLength = 0;
                fDSP->instanceClear();
                clearCurrentNote();
            }

            // Clear temporary outputs
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                delete [] outputs[channel];
            }
            delete [] outputs; 
        }
    }

    // Setters needed in main processor
    void setStopGain(float newStopGain){
        if (newStopGain == 0)
            offFlag = true;
        offFlag = false;
        stopGain.setTarget(newStopGain);
    }
    
    void setAttack(float newAttack){
        fUI->setParamValue("Attack", newAttack);
    }

private:
    // Faust system
    Faust::MapUI* fUI;
    Faust::dsp* fDSP;

    float fs, oneOverFs;
    bool offFlag = true;
    float tailLength;
    declicker<float> stopGain;

    Rank voiceRank;
    
    // Static coefs for different pipe frequencies
    static double principalDelay[73];
    static double principalCoefs[73][5];
    static double fluteDelay[73];
    static double fluteCoefs[73][5];
    static double violinDelay[73];
    static double violinCoefs[73][5];
    static double gedecktDelay[73];
    static double gedecktCoefs[73][5];

    // Private functions
    bool setMidi(int midiNoteNumber)
    {
        // Check rank type and do correct shift
        switch (voiceRank)
        {
            case PRINCIPAL8:
            case FLUTE8:
            case VIOLIN8:
            case GEDECKT8:
                break;
            case PRINCIPAL4:
            case FLUTE4:
            case VIOLIN4:
            case GEDECKT4:
                midiNoteNumber += 12;
                break;
            case PRINCIPAL16:
            case FLUTE16:
            case VIOLIN16:
            case GEDECKT16:
                midiNoteNumber -= 12;
                break;
            case PRINCIPAL513:
            case FLUTE513:
            case VIOLIN513:
            case GEDECKT513:
                midiNoteNumber += 7;
                break;
            case PRINCIPAL223:
            case FLUTE223:
            case VIOLIN223:
            case GEDECKT223:
                midiNoteNumber += 19;
                break;
            case PRINCIPAL2:
            case FLUTE2:
            case VIOLIN2:
                midiNoteNumber += 24;
                break;
        }
        
        // If outside available pipes return false
        if (midiNoteNumber < 24)
            return 0;
        if (midiNoteNumber > 96)
            return 0;
        
        // Set the MIDI note and ready to play
        fUI->setParamValue("MIDI Note", midiNoteNumber);
        fUI->setParamValue("Flow Gain", (std::log(74) + 2)/(std::log(1+midiNoteNumber) + 2));
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
    
    // Select the correct coef and delay compenstation
    void selectPipe(int midiNoteNumber)
    {
        int mInd = midiNoteNumber - 24;
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

        else if (voiceRank >= GEDECKT8 && voiceRank <= GEDECKT223)
        {
            b0 = gedecktCoefs[mInd][0]; b1 = gedecktCoefs[mInd][1]; b2 = gedecktCoefs[mInd][2];
            a1 = gedecktCoefs[mInd][3]; a2 = gedecktCoefs[mInd][4];
            delay = gedecktDelay[mInd];
        }
        
        fUI->setParamValue("b0", b0);
        fUI->setParamValue("b1", b1);
        fUI->setParamValue("b2", b2);
        fUI->setParamValue("a1", a1);
        fUI->setParamValue("a2", a2);
        fUI->setParamValue("Group Delay", delay);
    }
};
