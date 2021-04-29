# Pipe Organ Modeling

Pipe organ modeling for Spring 2021 MUSIC 320C

## File description

**pipe.dsp** - First implementation of digital waveguide pipe model

**SynthFramework** - Contains a JUCE project framework for using a Faust sound generator as a
synthesizer in JUCE. This is nice because it allows JUCE to handle polyphony. 

**OrganWaveguide** - Contains a JUCE project that uses a 1-D pipe digital waveguide model as it's synthesis engine. The resulting synthesizer contains multiple ranks (VCOs), each with polyphony.

## TO-DOs

1. More pipes! Currently have a Principal 4', 8', 16'. Could do 2 2/3', 2', or 5 1/3' easily.
Standard Flue Pipe (Should be a tweaking of waveguide one-pole):
* Flutes
* Strings
* Vox Celeste (dual flutes detuned by 2 Hz for T = 2 beat/s vibrato)
Reed Pipes (Requires new excitation source based on reed and conical horn geometry):
* Trumpets

2. Better excitation. Need better model of chiff using turbulence noise.

3. Better implementation
Increase Lagrange interpolater to 4th order, declick delay parameters. Clean up engine to ease of implementation. The werks...

