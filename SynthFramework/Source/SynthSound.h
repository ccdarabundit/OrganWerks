/*
  ==============================================================================

    SynthSound.h
    Created: 27 Apr 2021 9:41:31am
    Author:  Champ Darabundit

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;
class SynthSound : public SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNumber*/)
    {
        return true;
    }
    
    bool appliesToChannel(int /*midiChannel*/)
    {
        return true;
    }
};
