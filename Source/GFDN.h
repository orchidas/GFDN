/*
  ==============================================================================

    GFDN.h
    Created: 17 Sep 2021 12:31:02pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "FDN.h"
#include "MixingMatrix.h"
#include <Eigen/Dense>


class GFDN{
public:
    GFDN();
    ~GFDN();
    
    void initialize(int nGrp, float SR, int* nDel, int *LR, int* UR);
    void updateMixingMatrix(float mixingFrac, int whichRoom);
    void updateCoupledMixingMatrix();
    void updateT60Filter(float t60low, float t60high, float fT, int whichRoom);
    void updateDryMix(float dry);
    void updateCouplingCoeff(float alpha);          //construct block mixing matrix
    void updateListenerRoom(int whichRoom);
    void updateSourceRoom(int whichRoom);
    float processSample(const float input);
    
private:
    
    const float PI = std::acos(-1);
    int nGroups;
    FDN* fdns;
    int* nDelayLines;
    float* mixingAngles;
    float couplingCoeff;
    float dryMix;
    float* b;                                   //driving coefficients
    float* c;                                   //output coefficients
    int driverRoom;
    int sourceRoom;
    
    Eigen::Matrix2f couplingMatrix;
    Eigen::MatrixXf M_coup;
    Eigen::VectorXf delayLineOutput;
    Eigen::VectorXf delayLineInput;
    
    //MixingMatrix obj;
    

    
    
};
