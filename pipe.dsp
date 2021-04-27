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
import("stdfaust.lib");

freq = hslider("freq", 100, 20, 5000, 0.1): si.smoo;
gain = hslider("Gain", 1, 0, 1, 0.01): si.smoo;
feedbackGain = hslider("Feedback Gain", 0.4, 0, 5, 0.01): si.smoo;
gate = button("Gate"): int;
mix = hslider("Mix", 1, 0, 1, 0.01): si.smoo;
flowGain = hslider("Flow Gain", 1, 0.5, 10, 0.01): si.smoo;
jetOffset = hslider("Jet Offset", 0, -1.0, 1.0, 0.01): si.smoo;

c = 343; // Speed of sound
pipeDelay = (1./freq)*ma.SR;
pipeLength = attach((pipeDelay*c)/2 : vbargraph("Pipe Length [unit: m] [style:numerical]", 0, 10));
mouthDelay = pipeDelay/2;

// Tube-Reflection Filters
rFilt = fi.lowpass(1, 2000); // How does this get set

// Incoming noise jet
jet = no.noise * gate;

maxPipeDelay = (1./20)*ma.SR: int;
maxMouthDelay = maxPipeDelay/2 : int;
env = en.asr(0.1, 1, 0.1, gate)*0.5;
NL = _ : ma.tanh*-flowGain + jetOffset;
mouthDL = de.fdelay(maxMouthDelay, mouthDelay);
pipeDL = de.fdelay(maxPipeDelay, pipeDelay);
process = ((+<: pipeDL, _) ~(rFilt:pipeDL<:rFilt, (_*feedbackGain+jet:mouthDL:NL))): _*mix, _*
(1-mix) :+ :*(env)*gain <:_, _;
//process = ((_+jet: mouthDL : NL) : (+<: pipeDL, _) ~(rFilt:pipeDL<:rFilt)) : _*mix, _*(1-mix) :+;


