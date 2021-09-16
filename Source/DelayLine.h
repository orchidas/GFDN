/*
  ==============================================================================

    DelayLine.h
    Created: 3 Jul 2021 9:27:30pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "ShelfFilter.h"

//I referenced Jatin's FDN DelayUtils.h to update the DelayLine read and write pointers

class DelayLine{
public:
    DelayLine();
    ~DelayLine();
    
    void setFilterCoefficients(float gDC, float gPI, float fT);     //T60 filter at end of delayline
    void prepare(const int L, const float sampleRate);    //function to set delay line length
    float read();        //read from pointer
    void write(const float input);      //write a pointer
    void update();      //update Pointers
    
private:
    enum
    {
        maxDelay = 64*8192,
    };
    float delayBuffer[maxDelay];
    int readPtr = 0, writePtr = 0, length;
    ShelfFilter T60Filter;
};
