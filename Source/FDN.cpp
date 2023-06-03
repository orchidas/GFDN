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

void FDN::initialize(float SR, int nDel, int* delayLengths){
    
    nDelayLines = nDel;               //number of delay lines
    sampleRate = SR;                  //sampleRate
    mixingMatrix.resize(nDelayLines,nDelayLines);   //mixing matrix
    M.initialize(nDelayLines);        //mixing matrix object
    
    delayLineOutput.resize(nDelayLines);
    delayLineOutput.setZero();
    delayLineInput.resize(nDelayLines);
    delayLineInput.setZero();
    
    delLen = new int[nDelayLines];
    buffers = new DelayLine[nDelayLines];
    b = new float[nDelayLines];
    c = new float[nDelayLines];
    std::copy(delayLengths, delayLengths+nDel, delLen);
        
    
    for(int i = 0; i < nDelayLines; ++i){
        
        //std::cout << delLen[i] << std::endl;
        buffers[i].prepare(delLen[i], sampleRate);        //prepare the delay lines
        b[i] = -1 + 2 * ((float)std::rand())/RAND_MAX;    //random numbers between -1 and 1
        c[i] = -1 + 2 * ((float)std::rand())/RAND_MAX;    //output coefficients
    }
    
}

void FDN::updateMixingMatrix(float frac){
    
    mixingMatrix = M.updateMixingMatrix(frac);
    //std::cout << M.isOrthonormal(mixingMatrix) << std::endl;
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

MixingMatrix FDN::getMixingMatrix(){
    return M;
}

float FDN::getMixingAngle(){
    return M.getMixingAngle();
}

float FDN::processSample(const float input){
    
    
    float output = 0.0f;
    for(int i = 0; i < nDelayLines; i++){
        buffers[i].update();
        buffers[i].write(b[i]*input + delayLineInput(i));
        delayLineOutput(i) = buffers[i].read();
        output += c[i] * std::real(delayLineOutput(i));

    }
    
    output += d * input;
    delayLineInput = mixingMatrix * delayLineOutput;
    //std::cout << "Input : " << input << ", Output : " <<  output << std::endl;
    return output;
    
}
