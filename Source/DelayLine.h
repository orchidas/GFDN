/*
  ==============================================================================

    DelayLine.h
    Created: 3 Jul 2021 9:27:30pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "ShelfFilter.h"

class DelayLine{
public:
    DelayLine();
    ~DelayLine();
    
    void setFilterCoefficients(float gDC, float gPI, float fT);
    void prepare(const int L_ms, const float sampleRate);    //function to set delay line length
    float update(const float input); // push and pop from delay line

    
private:
    float* delayBuffer;
    int length;
    float sampleRate;
    ShelfFilter T60Filter;
    
    
};
