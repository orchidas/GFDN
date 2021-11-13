/*
  ==============================================================================

    DelayLine.cpp
    Created: 3 Jul 2021 9:27:30pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "DelayLine.h"


DelayLine::DelayLine(){}
DelayLine::~DelayLine(){}


void DelayLine::prepare(const int L, const float sampleRate){
    
    length = L;  //length of delay line in samples
    for (int i = 0; i < maxDelay; i++)     //initialize delay lines to prevent garbage memory values
        delayBuffer[i] = 0.0f;
    
    T60Filter.reset(sampleRate);     //filter at the end of the delay line
    
}

// update filter coefficients when parameters are changed by user
void DelayLine::setFilterCoefficients(float gDC, float gPI, float fT){
    T60Filter.updateCoeff(gDC, gPI, fT);
}
