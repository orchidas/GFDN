/*
  ==============================================================================

    CoupledMixingMatrix.cpp
    Created: 23 Oct 2021 5:44:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "CoupledMixingMatrix.h"

CoupledMixingMatrix::CoupledMixingMatrix(){};
CoupledMixingMatrix::~CoupledMixingMatrix(){};

void CoupledMixingMatrix::initialize(int nGrp, int nDel){
    
    nGroup = nGrp;
    nDelayLines = nDel;
    nSize = nDelayLines/nGroup;
    isFilter = false;
    
    M_block.resize(nDelayLines, nDelayLines);
    M_block.setIdentity();
    couplingScalars.resize(nDelayLines, nDelayLines);
    couplingScalars.setIdentity();
    couplingCoeff = 0.0f;
}


void CoupledMixingMatrix::updateBlockMatrix(Eigen::MatrixXf M_block_new){
    M_block = M_block_new;
}



void CoupledMixingMatrix::updateCouplingCoeff(float alpha){
    couplingCoeff = alpha * (PI/4.0f);
    
    updateCouplingFilters();
}


void CoupledMixingMatrix::updateCouplingFilters(){
    
    if (!isFilter){
        // for 2 coupled rooms only
        couplingMatrix2D << std::cos(couplingCoeff), std::sin(couplingCoeff),
        -std::sin(couplingCoeff), std::cos(couplingCoeff);
    
        for (int i = 0 ; i < nGroup; i++){
            for (int j = 0; j < nGroup; j++){
                for (int k = 0; k < nSize; k++){
                    for(int m = 0; m < nSize; m++)
                        couplingScalars(i*nSize+k, j*nSize+m) = couplingMatrix2D(i,j);
                }
            }
        }
    }
        
}


Eigen::VectorXf CoupledMixingMatrix::process(Eigen::VectorXf delayLineOutput){
    
    Eigen::VectorXf delayLineInput(delayLineOutput.size());
    //delayLineInput.setZero();
    delayLineInput = (M_block.cwiseProduct(couplingScalars)) * delayLineOutput;
    
    //doing this in a loop is too slow and breaks things
    /*for(int i = 0; i < nDelayLines; i++){
        for(int j = 0; j < nDelayLines; j++){
            if (!isFilter)
                delayLineInput(i) += M_block(i,j)*couplingScalars(i,j)*delayLineOutput(j);
            
        }
    }*/
    
    return delayLineInput;
}
