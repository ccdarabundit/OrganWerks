import("stdfaust.lib");

import("waveguides.lib");

process = waveguide(maxPipeDelay, 
initDelay, 
postDelay, 
rFilt,
tFilt,
outFilt1, 
outFilt2)~labium(gate, maxMouthDelay, mouthDelay) : !, _*(env)*gain <:_, _
with{
	midiNote = nentry("MIDI Note", 32, 24, 96, 1): int;
	gdelay = nentry("Group Delay", 0, 0, 1, 0.0000001);
	gain = nentry("Gain", 1, 0, 1, 0.01)*0.1: si.smoo;
	gate = button("Gate"): int;
	mix = 0.85;
	attack = nentry("Attack", 0.05, 0.0001, 0.5, 0.0001);
	b0 = nentry("b0", 1, 0, 2, 0.00001);
	b1 = nentry("b1", 0, 0, 2, 0.00001);
	b2 = nentry("b2", 0, 0, 2, 0.00001);
	a1 = nentry("a1", 0, 0, 2, 0.00001);
	a2 = nentry("a2", 0, 0, 2, 0.00001);

	freq = 2^( (midiNote - 69) / 12) * 440;
	c = 343; // Speed of sound
 	totalDelay = (ma.SR/freq);
	mouthDelay = totalDelay/2;

	bcoefs = (b0, b1, b2);
	acoefs = (a1, a2);
	rFilt = fi.iir(bcoefs, acoefs);
	tFilt = _*1.0;
	outFilt1 = _*(1-mix);
	outFilt2 = _*(mix);

	env = en.asr(attack, 1, 0.1, gate);
	maxPipeDelay = 0.5*(1./20)*ma.SR: int;
	maxMouthDelay = maxPipeDelay/2 : int;
	initDelay = totalDelay/2;
	postDelay = totalDelay-initDelay-gdelay*ma.SR - 2;
};