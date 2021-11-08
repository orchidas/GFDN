/*
  ==============================================================================

    CoupledMixingMatrix.cpp
    Created: 23 Oct 2021 5:44:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "CoupledMixingMatrix.h"

CoupledMixingMatrix::CoupledMixingMatrix(){};


CoupledMixingMatrix::~CoupledMixingMatrix(){
    delete [] PolyMat;
    delete [] coeffs;
};

void CoupledMixingMatrix::initialize(int nGrp, int totalDel, int *nDel){
    
    nGroup = nGrp;
    nDelayLines = totalDel;
    nSize = nDel;
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
        PolyMat = new Eigen::MatrixXcf [firOrder+1];
        coeffs = new std::complex<float> [firOrder+1];
        prevDelayLineOutput.resize(nDelayLines, firOrder+1);
        prevDelayLineOutput.setZero();
        
        perm.resize(firOrder+1);
        perm.indices() = {2,0,1};
        
   
        for(int i = 0; i < firOrder+1; i++){
            PolyMat[i].resize(nDelayLines, nDelayLines);
            PolyMat[i].setZero();
        }
        
//        couplingFilters = new FIRFilter*[nDelayLines];
//        for(int i = 0; i < nDelayLines; i++){
//            couplingFilters[i] = new FIRFilter[nDelayLines];
//            for (int j = 0; j < nDelayLines; j++)
//                couplingFilters[i][j].initialize(firOrder);
//
//        }
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
                for (int k = 0; k < nSize[i]; k++){
                    for(int m = 0; m < nSize[j]; m++)
                        couplingScalars(i*nSize[i]+k, j*nSize[j]+m) = couplingMatrix2D(i,j);
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
                
                for (int k = 0; k < nSize[i]; k++){
                    for(int m = 0; m < nSize[j]; m++){
                        //couplingFilters[i*nSize+k][j*nSize+m].setCoefficients(coeffs);

                        for (int p = 0; p < firOrder+1; p++)
                            PolyMat[p](i*nSize[i]+k, j*nSize[j]+m) = coeffs[p];
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
        
        //update previous filter inputs
        prevDelayLineOutput = prevDelayLineOutput * perm;
        prevDelayLineOutput.col(0) = delayLineOutput;
        
        //filter with polynomial FIR matrix
        Eigen::VectorXcf filterOutput(nDelayLines);
        filterOutput.setZero();
        
        //i am starting from 1 because i know coeffs[0] = 0
        for(int i = 1 ; i < firOrder+1; i ++){
            filterOutput += (M_block.cwiseProduct(PolyMat[i])) * prevDelayLineOutput.col(i);
        }
        
         //doing this in a loop is too slow
         /*for(int i = 0; i < nDelayLines; i++){
             for(int j = 0; j < nDelayLines; j++){
         
                 filterOutput(i) += M_block(i,j) * couplingFilters[i][j].process(delayLineOutput(j));
         
             }
         }*/
        
        delayLineInput = filterOutput;
       
    }
   
    return delayLineInput;
}
