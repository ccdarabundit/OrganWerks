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
#include <complex>

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
    // Order of original FIR, length of Delay Lines, Delay Index;
    int nTaps, lDelay, dInd;
    // 2D - Delay Line
    std::vector<std::complex<fType>> delayLine;
    // Filter modes, residues, and tail gain
    std::complex<fType> modes [2] = {0.f, 0.f};
    std::complex<fType> res [2] = {0.f, 0.f};
    std::complex<fType> tGain [2] = {0.f, 0.f};
    // Use two filter states for switching
    std::complex<zType> states0 [2] = {0.f, 0.f};
    std::complex<zType> states1 [2] = {0.f, 0.f};
    // Switch signal for filter states
    bool currFilter = 0;
    
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
        delayLine = std::vector<std::complex<fType>>(lDelay, 0);
        
        resetDSP();
        // Just in case ;)
        delayLine.shrink_to_fit();
    }
    // Update the coefficients
    void setCoefs(std::complex<fType> * newModes, std::complex<fType> * newRes, std::complex<fType> * newTGain)
    {
        memcpy(modes, newModes, sizeof(std::complex<fType>)*2);
        memcpy(res, newRes, sizeof(std::complex<fType>)*2);
        memcpy(tGain, newTGain, sizeof(std::complex<fType>)*2);
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
        std::fill(delayLine.begin(), delayLine.end(), 0);
        std::fill(states1, states1 + 2, std::complex<zType>(0.0, 0.0));
        std::fill(states0, states0 + 2, std::complex<zType>(0.0, 0.0));
    }
    
    // Compute function a la Faust
    void compute(int count, fType** inputs, fType** outputs)
    {
        // Assuming mono-in
        fType* input = inputs[0];
        fType* output = outputs[0];
        
        for (int i = 0; i < count; ++i)
        {
            // Grab delayed states
            std::complex<fType> dIn;
            delayLine[ (dInd) & (lDelay - 1)] = input[i];
            dIn = delayLine[ (dInd - nTaps) & (lDelay - 1)];
            
            // Output determined by the switch
            states1[0] = modes[0]*static_cast<std::complex<fType>>(states1[0]) + input[i] - tGain[0]*dIn;
            states1[1] = modes[1]*static_cast<std::complex<fType>>(states1[1]) + input[i] - tGain[1]*dIn;
            states0[0] = modes[0]*static_cast<std::complex<fType>>(states0[0]) + input[i] - tGain[0]*dIn;
            states0[1] = modes[1]*static_cast<std::complex<fType>>(states0[1]) + input[i] - tGain[1]*dIn;
            if (currFilter) // Filter 1
            {
                // State Update equation
                output[i] = (res[0]*static_cast<std::complex<fType>>(states1[0]) + res[1]*static_cast<std::complex<fType>>(states1[1])).real();
            }
            else    // Filter 0
            {
                output[i] = (res[0]*static_cast<std::complex<fType>>(states0[0]) + res[1]*static_cast<std::complex<fType>>(states0[1])).real();
            }
            
            // Sum initial and tail-canceling filter
        
            
            if (!(dInd % (nTaps+1)) && dInd > 0)
            {
                if (currFilter) // Clear filter state 1
                    std::fill(states1, states1 + 2, std::complex<zType>(0.0, 0.0));
                else
                    std::fill(states0, states0 + 2, std::complex<zType>(0.0, 0.0));


                // Switch filter
                currFilter = !currFilter;
            }
            // increment delay counter
            dInd++;
        }
    }
};

// 4 TIIR filters Overlap
template<class fType, class zType, int N=50>
class tiirModal4
{
private:
    // Delay Line Members
    // Order of original FIR, length of Delay Lines, Delay Index;
    int nTaps, lDelay, dInd, nStep;
    // 2D - Delay Line
    std::vector<std::complex<fType>> delayLine0;
    std::vector<std::complex<fType>> delayLine1;
    std::vector<std::complex<fType>> delayLine2;
    std::vector<std::complex<fType>> delayLine3;
    // Filter modes, residues, and tail gain
    std::complex<fType> modes [2] = {0.f, 0.f};
    std::complex<fType> res [2] = {0.f, 0.f};
    std::complex<fType> tGain [2] = {0.f, 0.f};
    // Use two filter states for switching
    std::complex<zType> states0 [2] = {0.f, 0.f};
    std::complex<zType> states1 [2] = {0.f, 0.f};
    std::complex<zType> states2 [2] = {0.f, 0.f};
    std::complex<zType> states3 [2] = {0.f, 0.f};
    // Switch signal for filter states
    int currFilter = 0;
    
public:
    tiirModal4()
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
        delayLine0 = std::vector<std::complex<fType>>(lDelay, 0);
        delayLine1 = std::vector<std::complex<fType>>(lDelay, 0);
        delayLine2 = std::vector<std::complex<fType>>(lDelay, 0);
        delayLine3 = std::vector<std::complex<fType>>(lDelay, 0);
        
        // get the step size by finding next multiple of 4
        nStep = nTaps;
        while (nStep % 4)
        {
            nStep++;
        }
        nStep = nStep / 4;
        resetDSP();
        // Just in case ;)
        delayLine0.shrink_to_fit();
        delayLine1.shrink_to_fit();
        delayLine2.shrink_to_fit();
        delayLine3.shrink_to_fit();
    }
    // Update the coefficients
    void setCoefs(std::complex<fType> * newModes, std::complex<fType> * newRes, std::complex<fType> * newTGain)
    {
        memcpy(modes, newModes, sizeof(std::complex<fType>)*2);
        memcpy(res, newRes, sizeof(std::complex<fType>)*2);
        memcpy(tGain, newTGain, sizeof(std::complex<fType>)*2);
    }
    

    void updateNTaps(int newTaps)
    {
        nTaps = newTaps;
        // get the step size by finding next multiple of 4
        nStep = nTaps;
        while (nStep % 4)
        {
            nStep++;
        }
        nStep = nStep / 4;
        resetDSP();
        // Clear the delay line
        void resetDSP();
    }
    
    // Clear all states
    void resetDSP()
    {
        dInd = 0;
        std::fill(delayLine0.begin(), delayLine0.end(), 0);
        std::fill(delayLine1.begin(), delayLine1.end(), 0);
        std::fill(delayLine2.begin(), delayLine2.end(), 0);
        std::fill(delayLine3.begin(), delayLine3.end(), 0);
        std::fill(states0, states0 + 2, std::complex<zType>(0.0, 0.0));
        std::fill(states1, states1 + 2, std::complex<zType>(0.0, 0.0));
        std::fill(states2, states2 + 2, std::complex<zType>(0.0, 0.0));
        std::fill(states3, states3 + 2, std::complex<zType>(0.0, 0.0));
    }
    
    // Compute function a la Faust
    void compute(int count, fType** inputs, fType** outputs)
    {
        // Assuming mono-in
        fType* input = inputs[0];
        fType* output = outputs[0];
        
        for (int i = 0; i < count; ++i)
        {
            // Check which state we're in and that we're not in the filter startup
            // If multiple of step we change output and reset delay lines
            if (!(dInd % (nStep)) && dInd >= 5*nStep)
            {
                switch(currFilter)
                {
                    case 0:
                    {
                        // Go to next state and clear current state
                        currFilter = 1;
                        std::fill(states0, states0 + 2, std::complex<zType>(0.0, 0.0));
                        std::fill(delayLine0.begin(), delayLine0.end(), 0);

                        break;
                    }
                    case 1:
                    {
                        // Go to next state and clear current state
                        currFilter = 2;
                        std::fill(states1, states1 + 2, std::complex<zType>(0.0, 0.0));
                        std::fill(delayLine1.begin(), delayLine1.end(), 0);

                        break;
                    }
                    case 2:
                    {
                        // Go to next state and clear current state
                        currFilter = 3;
                        std::fill(states2, states2 + 2, std::complex<zType>(0.0, 0.0));
                        std::fill(delayLine2.begin(), delayLine2.end(), 0);

                        break;
                    }
                    case 3:
                    {
                        // Go to next state and clear current state
                        currFilter = 0;
                        std::fill(states3, states3 + 2, std::complex<zType>(0.0, 0.0));
                        std::fill(delayLine3.begin(), delayLine3.end(), 0);

                        break;
                    }
                }
            }
            else if( dInd < 4*nStep)
            {
                if (dInd == 1*nStep)
                {
                    std::fill(states1, states1 + 2, std::complex<zType>(0.0, 0.0));
                    std::fill(delayLine1.begin(), delayLine1.end(), 0);

                }
                if (dInd == 2*nStep)
                {
                    std::fill(states2, states2 + 2, std::complex<zType>(0.0, 0.0));
                    std::fill(delayLine2.begin(), delayLine2.end(), 0);
                }
                if (dInd == 3*nStep)
                {
                    std::fill(states3, states3 + 2, std::complex<zType>(0.0, 0.0));
                    std::fill(delayLine3.begin(), delayLine3.end(), 0);
                }
            }
            // Grab delayed states
            std::complex<fType> d0, d1, d2, d3;
            
            // So we write the input to the same place
            delayLine0[ (dInd) & (lDelay - 1)] = input[i];
            delayLine1[ (dInd) & (lDelay - 1)] = input[i];
            delayLine2[ (dInd) & (lDelay - 1)] = input[i];
            delayLine3[ (dInd) & (lDelay - 1)] = input[i];
            
            // Grab our delayline values
            d0 = delayLine0[ (dInd - nTaps) & (lDelay - 1)];
            d1 = delayLine1[ (dInd - nTaps) & (lDelay - 1)];
            d2 = delayLine2[ (dInd - nTaps) & (lDelay - 1)];
            d3 = delayLine3[ (dInd - nTaps) & (lDelay - 1)];

            
            // Compute our states
    
            // Filter 0:
            states0[0] = modes[0]*static_cast<std::complex<fType>>(states0[0]) + input[i] - tGain[0]*d0;
            states0[1] = modes[1]*static_cast<std::complex<fType>>(states0[1]) + input[i] - tGain[1]*d0;
            
            // Filter 1:
            states1[0] = modes[0]*static_cast<std::complex<fType>>(states1[0]) + input[i] - tGain[0]*d1;
            states1[1] = modes[1]*static_cast<std::complex<fType>>(states1[1]) + input[i] - tGain[1]*d1;
            
            // Filter 2:
            states2[0] = modes[0]*static_cast<std::complex<fType>>(states2[0]) + input[i] - tGain[0]*d2;
            states2[1] = modes[1]*static_cast<std::complex<fType>>(states2[1]) + input[i] - tGain[1]*d2;
            
            // Filter 3:
            states3[0] = modes[0]*static_cast<std::complex<fType>>(states3[0]) + input[i] - tGain[0]*d3;
            states3[1] = modes[1]*static_cast<std::complex<fType>>(states3[1]) + input[i] - tGain[1]*d3;
            
            // Output Based on Switch
            switch (currFilter)
            {
                case 0:
                {
                    output[i] = (res[0]*static_cast<std::complex<fType>>(states0[0]) + res[1]*static_cast<std::complex<fType>>(states0[1])).real();
                    break;
                }
                case 1:
                {
                    output[i] = (res[0]*static_cast<std::complex<fType>>(states1[0]) + res[1]*static_cast<std::complex<fType>>(states1[1])).real();
                    break;
                }
                case 2:
                {
                    output[i] = (res[0]*static_cast<std::complex<fType>>(states2[0]) + res[1]*static_cast<std::complex<fType>>(states2[1])).real();
                    break;
                }
                case 3:
                {
                    output[i] = (res[0]*static_cast<std::complex<fType>>(states3[0]) + res[1]*static_cast<std::complex<fType>>(states3[1])).real();
                    break;
                }
            }
        
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
