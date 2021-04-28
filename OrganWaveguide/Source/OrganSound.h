/*
  ==============================================================================

    OrganSound.h
    Created: 27 Apr 2021 5:51:23pm
    Author:  Champ Darabundit

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

class OrganSound : public SynthesiserSound
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