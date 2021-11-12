/*
  ==============================================================================

    ShelfFilter.h
    Created: 3 Jul 2021 8:32:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <cmath>


class ShelfFilter{
public:
    
    ShelfFilter();
    ~ShelfFilter();
    
    void reset(const float sampleRate);
    void updateCoeff(float g_dc, float g_pi, float fT);
    float* getNumeratorCoefficients();
    float* getDenominatorCoefficients();
    
    //process each sample
    inline std::complex<float> process(const std::complex<float> input) {
        std::complex<float> output = b0 * input + b1 * prevInput - a1 * prevOutput;
        prevInput = input;
        prevOutput = output;

        return output;
    }
    
    
private:
    float b0, b1, a1;   //filter coefficients
    float gDC, gPI, gFT, wT;   //parameters input by user
    std::complex<float> prevInput = 0.0, prevOutput = 0.0;  // filter states
    const float PI = std::acos(-1);     //pi
    float sampleRate;                  //sampling frequency
    float* numCoeff;
    float* denCoeff;
    
};
