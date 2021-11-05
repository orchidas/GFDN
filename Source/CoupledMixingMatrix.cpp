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
    isFilter = true;
    
    M_block.resize(nDelayLines, nDelayLines);
    M_block.setIdentity();
    couplingCoeff = 0.0f;
    
    if (!isFilter){
        couplingScalars.resize(nDelayLines, nDelayLines);
        couplingScalars.setIdentity();
    }
    
    else{
        I.real(0); I.imag(1.0);
        couplingFilters = new FIRFilter*[nDelayLines];
        for(int i = 0; i < nDelayLines; i++){
            couplingFilters[i] = new FIRFilter[nDelayLines];
            for (int j = 0; j < nDelayLines; j++)
                couplingFilters[i][j].initialize(firOrder);
            
        }
    }
    
}


void CoupledMixingMatrix::updateBlockMatrix(Eigen::MatrixXf M_block_new){
    M_block = M_block_new;
}



void CoupledMixingMatrix::updateCouplingCoeff(float alpha){
    
    couplingCoeff = alpha * (PI/4.0f);
    updateCouplingFilters();
}


void CoupledMixingMatrix::updateBeta(float beta){
    
    this->beta = beta * (PI/2.0f);
    updateCouplingFilters();
}

void CoupledMixingMatrix::updateCouplingFilters(){
    
    // for 2 coupled rooms only
    couplingMatrix2D << std::cos(couplingCoeff), std::sin(couplingCoeff),
    -std::sin(couplingCoeff), std::cos(couplingCoeff);
    
    if (!isFilter){
        for (int i = 0 ; i < nGroup; i++){
            for (int j = 0; j < nGroup; j++){
                for (int k = 0; k < nSize; k++){
                    for(int m = 0; m < nSize; m++)
                        couplingScalars(i*nSize+k, j*nSize+m) = couplingMatrix2D(i,j);
                }
            }
        }
    }
    
    else{
        coeffs[0] = 0.0;
        for (int i = 0 ; i < nGroup; i++){
            for (int j = 0; j < nGroup; j++){
                if (i == j){
                    if (i == 0){
                        coeffs[1] = couplingMatrix2D(0,0) * couplingMatrix2D(0,0);
                        coeffs[2] = std::exp(beta*I) * couplingMatrix2D(0,1) * couplingMatrix2D(0,1);
                    }
                    else{
                        coeffs[1] = couplingMatrix2D(0,1) * couplingMatrix2D(0,1);
                        coeffs[2] = std::exp(I*beta) * couplingMatrix2D(0,0) * couplingMatrix2D(0,0);
                    }
                }
                else{
                    coeffs[1] = pow(-1,i) * couplingMatrix2D(0,0) * couplingMatrix2D(i,j);
                    coeffs[2] = -std::exp(I*beta) * coeffs[1];
                }
                
                for (int k = 0; k < nSize; k++){
                    for(int m = 0; m < nSize; m++){
                        couplingFilters[i*nSize+k][j*nSize+m].setCoefficients(coeffs);
                    }
                }
            }
        }
        
    }
}


Eigen::VectorXcf CoupledMixingMatrix::process(Eigen::VectorXcf delayLineOutput){
    

    Eigen::VectorXcf delayLineInput(nDelayLines);
    
    if (!isFilter)
        delayLineInput = (M_block.cwiseProduct(couplingScalars)) * delayLineOutput;
    
    else{
        
        Eigen::VectorXcf filterOutput(nDelayLines);
        filterOutput.setZero();
        
         //doing this in a loop is too slow and breaks things
         for(int i = 0; i < nDelayLines; i++){
             for(int j = 0; j < nDelayLines; j++){
         
                 filterOutput(i) += M_block(i,j) * couplingFilters[i][j].process(delayLineOutput(j));
         
             }
         }
        
        delayLineInput = filterOutput;
       
    }
   
    return delayLineInput;
}
