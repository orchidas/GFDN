/*
  ==============================================================================

    FIRFilter.cpp
    Created: 5 Nov 2021 4:33:01pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "FIRFilter.h"


FIRFilter::FIRFilter(){}

FIRFilter::~FIRFilter(){
    delete [] b;
    delete [] prevInput;
}

void FIRFilter::initialize(int order){
    
    this->order = order;
    b = new std::complex<float> [order+1];
    prevInput = new std::complex<float> [order];
    //initialize to prevent garbage values
    for(int i = 0; i < order; i++){
        prevInput[i].real(0.0);
        prevInput[i].imag(0.0);
    }
    
}


void FIRFilter::setCoefficients(std::complex<float> *coeffs){
    std::copy(coeffs, coeffs+order+1, b);
}


std::complex<float> FIRFilter::process(std::complex<float> input){
    
    std::complex<float> output = 0.0;
    
    for(int i = 0; i < order; i++){
        output += b[i+1]*prevInput[i];
    }
    
    //update prevInput buffer - right shift by 1
    for(int i = order-1; i > 0; i--){
        prevInput[i] = prevInput[i-1];
    }
    prevInput[0] = input;
    return output;
    
}

