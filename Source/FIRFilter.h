/*
  ==============================================================================

    FIRFilter.h
    Created: 5 Nov 2021 4:33:01pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include <iostream>
#include <complex>
#include <cmath>


class FIRFilter{
public:
    FIRFilter();
    ~FIRFilter();
    
    void initialize(int order);
    void setCoefficients(std::complex<float>* coeffs);
    std::complex<float> process(std::complex<float> input);
    
private:
    
    int order;
    std::complex<float>* b;
    std::complex<float>* prevInput;
    const float PI = std::acos(-1);
    
};
