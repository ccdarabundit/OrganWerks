/**************************************************
 ; TIIR - Truncated Infinite Impulse Response Filter
 ; Also known as "Tail-Canceling IIRs" see
 ;
 ; Wang and Smith, "On Fast FIR Filters Implemented as Tail-Canceling IIR Filters"
 ;
 ; Allows for a more efficient (at the cost of memory) implementation of an FIR filter
 ; and also a stable implementation of growing exponential response.
 ;
 ; Currently we assume one poles for both filters
 ; *************************************************
 ; Champ Darabundit - 05/14/2021
 ; Copyright © 2021 Champ Darabundit. All rights reserved.
 ***************************************************/

#ifndef TIIR_h
#define TIIR_h
#include <vector>

// Biquad template for use in TIIR
template<class fType>
class biquad{
private:
    // For biquad we assume coef structure
    // [ b0, b1, b2, a1, a2 ]
    // Intialize to a bypass filter
    fType coefs[5] = {1.f, 0.0, 0.0, 0.0, 0.0};
    fType states[2] = {0.f, 0.f};
public:
    biquad() {} // Constructor does nothing
    // Getters and setters
    void setCoefs(fType * newCoefs)
    {
        memcpy(coefs, newCoefs, sizeof(fType)*5);
    }
    fType * getCoefs()
    {
        return &coefs[0];
    }
    void setStates(fType * newStates)
    {
        memcpy(states, newStates, sizeof(fType)*2);
    }
    void setStates(fType s0, fType s1)
    {
        states[0] = s0;
        states[1] = s1;
    }
    fType * getStates()
    {
        return &states[0];
    }
    // Reset Function
    void resetDSP()
    {
        memset(states, 0.f, sizeof(fType)*2);
    }
    // Process function
    fType processSample(fType input)
    {
        fType output = coefs[0]*input + states[0];
        states[0] = coefs[1]*input - coefs[3]*output + states[1];
        states[1] = coefs[2]*input - coefs[4]*output;
        return output;
    }
};

// TIIR template
// Initialize with maximum delay length
template<class fType, int N=50>
class tiir
{
private:
    // Delay Line Members
    // Order of original FIR, length of delay, delay index;
    int nTaps, lDelay, dInd;
    std::vector<fType> delayLine;   // Delay line for cancelation
    // Filter Members
    biquad<fType> init;
    biquad<fType> tail;
    
public:
    tiir()
    {
        nTaps = N;
        // Get nearest power of two
        uint32_t dlSize = N - 1;
        dlSize |= dlSize >> 1;
        dlSize |= dlSize >> 2;
        dlSize |= dlSize >> 4;
        dlSize |= dlSize >> 8;
        dlSize |= dlSize >> 16;
        lDelay = dlSize + 1;
        delayLine = std::vector<fType>(lDelay, 0);
        resetDSP();
        // Just in case ;)
        delayLine.shrink_to_fit();
    }
    // Update the coefficients
    void setCoefs(fType * initCoefs, fType * tailCoefs)
    {
        init.setCoefs(initCoefs);
        tail.setCoefs(tailCoefs);
    }
    
    // Change delay line length
    // Not to be used on the fly because
    // we zero delay line and states
    void updateNTaps(int newTaps)
    {
        nTaps = newTaps;
        // Clear the delay line
        void resetDSP();
    }
    
    // Clear all states
    void resetDSP()
    {
        dInd = 0;
        std::fill(delayLine.begin(), delayLine.end(), 0);
        init.resetDSP(); tail.resetDSP();
    }
    
    // Compute function a la Faust
    void compute(int count, fType** inputs, fType** outputs)
    {
        // Assuming mono-in
        fType* input = inputs[0];
        fType* output = outputs[0];
        for (int i = 0; i < count; ++i)
        {
            // write input to our delay line
            delayLine[dInd & (lDelay - 1)] = static_cast<fType>( input[i] );
            // Grabed the delayed input for tail cancelation
            fType dIn = delayLine[ (dInd - nTaps) & (lDelay - 1)];
            // Call process from IIR filters
            output[i] = init.processSample(input[i]) - tail.processSample(dIn);
            dInd++;
        }
    }
};

// TIIR template
// Initialize with maximum delay length
template<class fType, class zType, int N=50>
class tiirModal
{
private:
    // Delay Line Members
    // Order of original FIR, length of delay, delay index;
    int nTaps, lDelay, dInd;
    std::vector<zType> stateDelay0;   // Delay line for cancelation
    std::vector<zType> stateDelay1;   // Delay line for cancelation
    // Filter modes, residues, and tail gain
    fType modes [2] = {0.f, 0.f};
    fType res [2] = {0.f, 0.f};
    fType tGain [2] = {0.f, 0.f};
    zType states [2] = {0.f, 0.f};
    
public:
    tiirModal()
    {
        nTaps = N;
        // Get nearest power of two
        uint32_t dlSize = N - 1;
        dlSize |= dlSize >> 1;
        dlSize |= dlSize >> 2;
        dlSize |= dlSize >> 4;
        dlSize |= dlSize >> 8;
        dlSize |= dlSize >> 16;
        lDelay = dlSize + 1;
        stateDelay0 = std::vector<zType>(lDelay, 0);
        stateDelay1 = std::vector<zType>(lDelay, 0);
        resetDSP();
        // Just in case ;)
        stateDelay0.shrink_to_fit();
        stateDelay1.shrink_to_fit();
    }
    // Update the coefficients
    void setCoefs(fType * newModes, fType * newRes, fType * newTGain)
    {
        memcpy(modes, newModes, sizeof(fType)*2);
        memcpy(res, newRes, sizeof(fType)*2);
        memcpy(tGain, newTGain, sizeof(fType)*2);
    }
    

    void updateNTaps(int newTaps)
    {
        nTaps = newTaps;
        // Clear the delay line
        void resetDSP();
    }
    
    // Clear all states
    void resetDSP()
    {
        dInd = 0;
        std::fill(stateDelay0.begin(), stateDelay0.end(), 0);
        memset(states, 0.f, sizeof(zType)*2);
    }
    
    // Compute function a la Faust
    void compute(int count, fType** inputs, fType** outputs)
    {
        // Assuming mono-in
        fType* input = inputs[0];
        fType* output = outputs[0];
        
        for (int i = 0; i < count; ++i)
        {
            // State Update equation
            states[0] = modes[0]*states[0] + res[0]*input[i];
            states[1] = modes[1]*states[1] + res[1]*input[i];
            
            // Write states to delay line
            stateDelay0[dInd & (lDelay - 1)] = static_cast<zType>( states[0] );
            stateDelay1[dInd & (lDelay - 1)] = static_cast<zType>( states[1] );
            
            // Grab delayed states
            zType ds0, ds1;
            ds0 = stateDelay0[ (dInd - nTaps) & (lDelay - 1)];
            ds1 = stateDelay1[ (dInd - nTaps) & (lDelay - 1)];
            fType sub = (tGain[0]*ds0 + tGain[1]*ds1);
            fType main = states[0] + states[1];
            // Output Update Equation
            output[i] = main - sub;
            
            // increment delay counter
            dInd++;
        }
    }
};

// TIIR++
// Truncated IIR with decaying IIR tail
// Great because they can share a delayline
template<class fType, int N=50>
class tiirpp
{
private:
    // Order of original FIR, length of delay, delay index;
    int nTaps, lDelay, dInd;
    // TIIR Filter coefficients
    // b0, a1, bZ
    fType tiirCoefs[3];
    // IIR tail coefs
    // b0, b1, a1
    fType iirCoefs[3];
    fType sT, sI;                       // Filter states
    std::vector<fType> delayLine;   // Delay line for cancelation
public:
    tiirpp()
    {
        nTaps = N;
        // Get nearest power of two
        uint32_t dlSize = N - 1;
        dlSize |= dlSize >> 1;
        dlSize |= dlSize >> 2;
        dlSize |= dlSize >> 4;
        dlSize |= dlSize >> 8;
        dlSize |= dlSize >> 16;
        lDelay = dlSize + 1;
        delayLine = std::vector<fType>(lDelay, 0);
        resetDSP();
        // Just in case ;)
        delayLine.shrink_to_fit();
        // Set filter coefs to bypass
        for (int i = 0; i < 3; ++i)
        {
            tiirCoefs[i] = 0;
            iirCoefs[i] = 0;
        }
        // Set states to 0
    }
    // Update the coefficients
    void updateCoefs(fType * newTIIRCoefs, fType * newIIRCoefs)
    {
        std::memcpy(tiirCoefs, newTIIRCoefs, sizeof(fType)*3);
        std::memcpy(iirCoefs, newIIRCoefs, sizeof(fType)*3);
    }
    
    // Change delay line length
    // Not to be used on the fly because
    // we zero delay line and states
    void updateNTaps(int newTaps)
    {
        nTaps = newTaps;
        void resetDSP();
    }
    
    // Clear all states
    void resetDSP()
    {
        dInd = 0; sT=0; sI = 0;
        std::fill(delayLine.begin(), delayLine.end(), 0);
    }
    
    // Compute function a la Faust
    void compute(int count, fType** inputs, fType** outputs)
    {
        // Assuming mono-in
        fType* input = inputs[0];
        fType* output = outputs[0];
        for (int i = 0; i < count; ++i)
        {
            // write input to our delay line
            delayLine[dInd & (lDelay - 1)] = static_cast<fType>( input[i] );
            // Grabed the delayed input for tail cancelation
            fType dIn = delayLine[ (dInd - nTaps) & (lDelay - 1)];
            // TIIR processing
            float tiirOut = tiirCoefs[0]*input[i] - tiirCoefs[2]*dIn - tiirCoefs[1]*sT;
            // Write the state
            sT = tiirOut;
            
            // IIR processing DF2-T
            float iirOut = iirCoefs[0]*dIn + sI;
            sI = iirCoefs[1]*dIn + iirCoefs[2]*iirOut;
            
            output[i] = tiirOut + iirOut;
            
            // Increment delay line index
            dInd++;
        }
    }
    
    // In practice we want two outputs
    // One at the end of the TIIR segment and one at the end of the
    // IIR segment
    void compute2(int count, fType** inputs, fType** tiirOutputs, fType** outputs)
    {
        // Assuming mono-in
        fType* input = inputs[0];
        fType* tiirOutput = tiirOutputs[0];
        fType* output = outputs[0];
        for (int i = 0; i < count; ++i)
        {
            // write input to our delay line
            delayLine[dInd & (lDelay - 1)] = static_cast<fType>( input[i] );
            // Grabed the delayed input for tail cancelation
            fType dIn = delayLine[ (dInd - nTaps) & (lDelay - 1)];
            // TIIR processing
            float tiirOutput[i] = tiirCoefs[0]*input[i] - tiirCoefs[2]*dIn - tiirCoefs[1]*sT;
            // Write the state
            sT = tiirOutput[i];
            
            // IIR processing DF2-T
            float iirOut = iirCoefs[0]*dIn + sI;
            sI = iirCoefs[1]*dIn + iirCoefs[2]*iirOut;
            
            output[i] = tiirOutput[i] + iirOut;
            
            // Increment delay line index
            dInd++;
        }
    }
};
#endif /* TIIR_h */
