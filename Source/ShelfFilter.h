/*
  ==============================================================================

    ShelfFilter.h
    Created: 3 Jul 2021 8:32:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include <iostream>
#include <cmath>


class ShelfFilter{
public:
    
    ShelfFilter();
    ~ShelfFilter();
    
    void reset(const float sampleRate);
    void updateCoeff(float g_dc, float g_pi, float fT);
    float process(const float input);
    
    
private:
    float b0, b1, a1;   //filter coefficients
    float gDC, gPI, gFT, wT;   //parameters input by user
    float  prevInput = 0.0f, prevOutput = 0.0f;  // filter states
    const float PI = std::acos(-1);     //pi
    float sampleRate;                  //sampling frequency
    
    
};
