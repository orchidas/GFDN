/*
  ==============================================================================

    DelayLine.cpp
    Created: 3 Jul 2021 9:27:30pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "DelayLine.h"

//constructor and destructor
DelayLine::DelayLine(){}
DelayLine::~DelayLine(){
    delete [] delayBuffer;
}


//initialize delay line
void DelayLine::prepare(const int L, const float sampleRate){
    
    length = L;  //length of delay line in samples
    delayBuffer = new float[length];
    for (int i = 0; i < L; i++)     //initializa delay lines to prevent garbage memory values
        delayBuffer[i] = 0.0f;
    
    T60Filter.reset(sampleRate);     //filter at the end of the delay line

}

// update filter coefficients when parameters are changed by user
void DelayLine::setFilterCoefficients(float gDC, float gPI, float fT){
    T60Filter.updateCoeff(gDC, gPI, fT);
}

// push at the start, pop from the end, shift all delay line elements to the right
// finally, pass through T60 filter
float DelayLine::update(const float input){
    
    float output = delayBuffer[length-1];
    for (int i = 0; i < length-1; i++){
        delayBuffer[i+1] = delayBuffer[i];
    }
    delayBuffer[0] = input;
    return T60Filter.process(output);
}
