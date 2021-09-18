/*
  ==============================================================================

    ShelfFilter.cpp
    Created: 3 Jul 2021 8:32:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "ShelfFilter.h"


//constructor and destructor
ShelfFilter:: ShelfFilter(){}
ShelfFilter::~ShelfFilter(){}

// reset filter at the start
void ShelfFilter::reset(const float sampleRate){
    b0 = 0.0f; b1 = 0.0f; a1 = 0.0f;
    prevInput = 0.0f; prevOutput = 0.0f;
    this->sampleRate = sampleRate;
}


// update shelf filter coefficients based on input DC and
// Nyquist gains and transition frequency in Hz
void ShelfFilter::updateCoeff(float g_dc, float g_pi, float fT){
    
    gDC = std::abs(g_dc);
    gPI = std::abs(g_pi);
    gFT = std::sqrt(gDC * gPI);
    
    wT = PI*(fT/(sampleRate/2.0));
    
    float alpha = 0.0f;
    if (gPI != gDC){
        float lambda = (gPI + gDC)/(gPI - gDC);
        int sign = signbit(lambda) == 0? 1:-1;
        alpha = lambda - (float)sign*std::sqrt(pow(lambda,2) - 1);
    }
    
    float beta0 = 0.5 * ((gDC + gPI) + (gDC - gPI) * alpha);
    float beta1 = 0.5 * ((gDC - gPI) + (gDC + gPI) * alpha);
    
    float rho = std::sin(wT/2.0 - PI/4.0) / std::sin(wT/2.0 + PI/4.0);
    b0 = (beta0 + rho*beta1) / (1 + rho*alpha);
    b1 = (beta1 + rho*beta0) / (1 + rho*alpha);
    a1 = (rho + alpha)/(1 + rho*alpha);
    
    //std::cout << "b0 :" << b0 << " b1 :" << b1 << " a1 :" << a1 << std::endl;
}


//process each sample
float ShelfFilter::process(const float input){
    float output = b0*input + b1*prevInput - a1*prevOutput;
    prevInput = input;
    prevOutput = output;
    
    return output;
}





