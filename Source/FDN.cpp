/*
  ==============================================================================

    FDN.cpp
    Created: 8 Jul 2021 7:10:17pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "FDN.h"
#include <Eigen/Dense>

FDN::FDN(){}
FDN::~FDN(){
    delete [] delLen;
    delete [] buffers;
    delete [] b;
    delete [] c;
}

void FDN::initialize(float SR, int nDel){
    
    nDelayLines = nDel;               //number of delay lines
    sampleRate = SR;                  //sampleRate
    M.initialize(nDelayLines);        //mixing matrix
    
    delayLineOutput.resize(nDelayLines);
    delayLineOutput.setZero();
    delayLineInput.resize(nDelayLines);
    delayLineInput.setZero();
    
    delLen = new int[nDelayLines];
    buffers = new DelayLine[nDelayLines];
    b = new float[nDelayLines];
    c = new float[nDelayLines];
    
    for(int i = 0; i < nDelayLines; ++i){
        
        float L_ms = 5 + (std::rand() % (20-5+1));   //delay line lengths between 5 and 20 ms
        delLen[i] = (int)(L_ms * sampleRate/1000.0);
        buffers[i].prepare(delLen[i], sampleRate);        //prepare the delay lines
        b[i] = -1 + 2 * ((float)std::rand())/RAND_MAX;                 //random numbers between -1 and 1
        c[i] = 1.0f;                                //output coefficients = 1
        //std::cout << "Driving coeffs " << b[i] << std::endl;
    }
    
}

void FDN::updateMixingMatrix(float frac){
    
    M.updateMixingMatrix(frac);
}


void FDN::updateT60Filter(float t60low, float t60high, float fT){
    
    for (int i = 0; i < nDelayLines; i++){
        
        float gDC = std::exp(-6.91 * (delLen[i]/sampleRate)/t60low);
        float gPI = std::exp(-6.91 * (delLen[i]/sampleRate)/t60high);
        buffers[i].setFilterCoefficients(gDC, gPI, fT);
    }
}


void FDN::updateDryMix(float dry){
    d = dry;
}


float FDN::processSample(const float input){
    
    float output = 0.0f;
    for (int i = 0; i < nDelayLines; i++){
        delayLineOutput(i) =  buffers[i].update(b[i]*input + delayLineInput(i));
        output += c[i] * delayLineOutput(i) + d * input;
    }
    
    delayLineInput = M.getMixingMatrix() * delayLineOutput;
    //std::cout << "Input : " << input << ", Output : " <<  output << std::endl;
    return output;
    
}



