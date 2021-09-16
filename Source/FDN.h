/*
  ==============================================================================

    FDN.h
    Created: 8 Jul 2021 7:10:17pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "DelayLine.h"
#include "MixingMatrix.h"
#include <Eigen/Dense>

class FDN{
public:
    
    FDN();                      //constructor
    ~FDN();                     //destructor
    
    void initialize(float SR, int nDel);  //initialize FDN, setup delayLine buffers
    
    void updateMixingMatrix(float frac);     //update mixing matrix when user changes parameters
    
    void updateT60Filter(float t60low, float t60high, float fT);    //update absorption filter at the end of delay lines
    
    void updateDryMix(float dry);           //update direct path value
    
    float processSample(const float input);      //spit out one sample of output based on incoming input
    
    Eigen::MatrixXf kroneckerProduct(Eigen::MatrixXf m1, Eigen::MatrixXf m2);   //compute kronecker product between 2 eigen matrices
    

private:
    int nDelayLines;                            //number of delay lines
    float sampleRate;                           //sample rate in Hz
    DelayLine* buffers;                         //all the delay line buffers
    int* delLen;                                //length of each delay line
    float* b;                                   //driving coefficients
    float* c;                                   //output coefficients
    float d;                                    //direct path
   
    
    //matrices and vectors
    MixingMatrix M;
    Eigen::VectorXf delayLineOutput;
    Eigen::VectorXf delayLineInput;
    
    
};


