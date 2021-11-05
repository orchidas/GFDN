/*
  ==============================================================================

    CoupledMixingMatrix.h
    Created: 23 Oct 2021 5:44:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <Eigen/Dense>
#include <complex>
#include <cmath>
#include "FIRFilter.h"
#include "ShelfFilter.h"


class CoupledMixingMatrix{
public:
    CoupledMixingMatrix();
    ~CoupledMixingMatrix();
    
    void initialize(int nGrp, int nDel);
    void updateBlockMatrix(Eigen::MatrixXf M_block_new);
    void updateCouplingCoeff(float alpha);
    void updateBeta(float beta);
    void updateCouplingFilters();
    Eigen::VectorXcf process(Eigen::VectorXcf delayLineOutput);
    
private:
    Eigen::MatrixXf M_block;
    Eigen::MatrixXf couplingScalars;
    Eigen::Matrix2f couplingMatrix2D;
    FIRFilter** couplingFilters;    //2D array of coupling Filters
    const int firOrder = 2;
    
    
    int nGroup, nDelayLines, nSize;
    bool isFilter;
    float couplingCoeff;
    const float PI = std::acos(-1);
    float beta;     //diffraction filter cutoff, ranges between 0 and pi/2
    std::complex<float> I;  //square root of negative 1
    std::complex<float> coeffs[3];
    
    
};
