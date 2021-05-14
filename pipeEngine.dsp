declare description "Waveguide Pipe";
declare author "Champ Darabundit";
declare version "0.9";

// VERY VERY SIMPLE WAVEGUIDE MODEL
// Based off of
// Valimaki et. al. "A Real-Time DSP Implementation of a Flute Model"
// Which is based off of
// M.E. McIntyre et. al. "On the oscillations of musical instruments"
// Fletcher and Rossing book is a good reference, but the subtleties of the model expressed there
// are not used here
// Structure is referenced from Romain's faust stk

// Pipe Engine Model, for use in JUCE
// Less control, and all sliders are now entries
// Champ Darabundit, 04/27/21
import("stdfaust.lib");

freq = nentry("freq", 100, 20, 5000, 0.1): si.smoo;
gain = nentry("gain", 1, 0, 1, 0.01): si.smoo;
feedbackGain = 0.25; //nentry("Feedback Gain", 0.4, 0, 5, 0.01): si.smoo;
gate = button("Gate"): int;
mix = nentry("Mix", 0.5, 0, 1, 0.01): si.smoo;
freqLimit = freq > 500;
flowGain = 0.5, freq/1000: select2(freqLimit);
jetOffset = nentry("Jet Offset", 0, -1.0, 1.0, 0.01): si.smoo;

c = 343; // Speed of sound
pipeDelay = (1./freq)*ma.SR: si.smoo;
pipeLength = 2.0*(freq/c);
pipeRadius = (pipeLength*0.3)/2;
mouthDelay = pipeDelay/2;
cutoff = 4*(2*3.14159*freq*0.3*pipeLength)/(c/freq);
// Tube-Reflection Filters
// Set it so that it is always
rFilt = fi.lowpass(1, 2000); // How does this get set

// Incoming noise jet
jet = no.noise * gate;

maxPipeDelay = (1./20)*ma.SR: int;
maxMouthDelay = maxPipeDelay/2 : int;
env = en.asr(0.1, 1, 0.1, gate)*0.5;
NL = _ : ma.tanh*-flowGain + jetOffset;
mouthDL = de.fdelay4(maxMouthDelay, mouthDelay);
pipeDL = de.fdelay4(maxPipeDelay, pipeDelay);
process = ((+<: pipeDL, _) ~(rFilt:pipeDL<:rFilt, (_*feedbackGain+jet:mouthDL:NL))): _*mix, _*
(1-mix) :+ :*(env)*gain <:_, _;
//process = ((_+jet: mouthDL : NL) : (+<: pipeDL, _) ~(rFilt:pipeDL<:rFilt)) : _*mix, _*(1-mix) :+;


