/*
  ==============================================================================

    DelayLine.h
    Created: 3 Jul 2021 9:27:30pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "ShelfFilter.h"

class DelayLine{
public:
    DelayLine();
    ~DelayLine();
    
    void setFilterCoefficients(float gDC, float gPI, float fT);     //T60 filter at end of delayline
    void prepare(const int L, const float sampleRate);    //function to set delay line length

    //read from pointer
    inline std::complex<float> read() const noexcept {
        return delayBuffer[readPtr];
    }

    //write a pointer
    inline void write(const std::complex<float> input) {

        delayBuffer[writePtr] = T60Filter.process(input);
    }

    //update Pointers: I referenced Jatin's FDN DelayUtils.h to update the DelayLine read and write pointers
    inline void update() {
        --writePtr;

        if (writePtr < 0) // wrap write pointer
            writePtr = maxDelay - 1;

        readPtr = writePtr + length;
        if (readPtr >= maxDelay) // wrap read pointer
            readPtr -= maxDelay;
    }
    
private:
    enum
    {
        maxDelay = 32*8192,
    };
    std::complex<float> delayBuffer[maxDelay];
    int readPtr = 0, writePtr = 0, length;
    ShelfFilter T60Filter;
};