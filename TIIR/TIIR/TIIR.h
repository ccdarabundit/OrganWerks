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
// TIIR template
// Initialize with maximum delay length
template<class fType, int N=50>
class tiir
{
private:
    // Order of original FIR, length of delay, delay index;
    int nTaps, lDelay, dInd;
    fType b0, a1, bZ;               // Filter coefficients
    fType s0;                       // Filter states
    std::vector<fType> delayLine;   // Delay line for cancelation
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
        // Set filter coefs to Bypass
        b0 = 1; a1 = 0; bZ = 0;
    }
    // Update the coefficients
    void updateCoefs(fType newB0, fType newA1, fType newBZ)
    {
        b0 = newB0; a1 = newA1; bZ = newBZ;
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
        dInd = 0; s0 = 0;
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
            output[i] = b0*input[i] - bZ*dIn - a1*s0;
            // Write the state
            s0 = output[i];
            // Increment delay line index
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
