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

//initialize delay line
void DelayLine::prepare(const int L, const float sampleRate){
    
    length = L;  //length of delay line in samples
    for (int i = 0; i < maxDelay; i++)     //initializa delay lines to prevent garbage memory values
        delayBuffer[i] = 0.0f;
    
    T60Filter.reset(sampleRate);     //filter at the end of the delay line
    
}

// update filter coefficients when parameters are changed by user
void DelayLine::setFilterCoefficients(float gDC, float gPI, float fT){
    T60Filter.updateCoeff(gDC, gPI, fT);
}

float DelayLine::read(){
 
    return T60Filter.process(delayBuffer[readPtr]);
}

void DelayLine::write(const float input){
  
    delayBuffer[writePtr] = input;
}

void DelayLine::update(){
    --writePtr;
    
    if (writePtr < 0) // wrap write pointer
        writePtr = maxDelay - 1;
    
    readPtr = writePtr + length;
    if (readPtr >= maxDelay) // wrap read pointer
        readPtr -= maxDelay;
}
