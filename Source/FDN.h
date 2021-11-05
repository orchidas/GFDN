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
    
    void initialize(float SR, int nDel, int LR, int UR);  //initialize FDN, setup delayLine buffers
    
    void updateMixingMatrix(float frac);     //update mixing matrix when user changes parameters
    
    void updateT60Filter(float t60low, float t60high, float fT);    //update absorption filter at the end of delay lines
    
    void updateDryMix(float dry);           //update direct path value
    
    float processSample(const float input);      //spit out one sample of output based on incoming input
    
    MixingMatrix getMixingMatrix();
    
    void findNPrime(int LR, int UR, int N);   //find N prime numbers within a range
    
    float getMixingAngle();
    
    DelayLine* buffers;                         //all the delay line buffers
    int* delLen;                                //length of each delay line
    float* b;                                   //driving coefficients
    float* c;                                   //output coefficients
   

private:
    int nDelayLines;                            //number of delay lines
    float sampleRate;                           //sample rate in Hz
    float d;                                    //direct path
   
    
    //matrices and vectors
    MixingMatrix M;
    Eigen::MatrixXf mixingMatrix;
    Eigen::VectorXcf delayLineOutput;
    Eigen::VectorXcf delayLineInput;
   
    
    
};


