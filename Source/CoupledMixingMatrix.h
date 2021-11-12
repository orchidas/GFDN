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
    
    void initialize(int nGrp, int totalDel, int* nDel);
    void updateBlockMatrix(Eigen::MatrixXf M_block_new);
    void updateCouplingCoeff(float alpha);
    void updateBeta(float beta);
    void updateCouplingFilters();
    Eigen::VectorXcf process(Eigen::VectorXcf delayLineOutput);
    
    void preComputeFilterVariables();

private:
    static constexpr int nDelayLines = 16; // total number of delay lines
    static constexpr int firOrder = 2;     //polynomial matrix order
    using cf = std::complex<float>;
    
    Eigen::Matrix<cf, nDelayLines, nDelayLines> M_block;
    Eigen::Matrix<cf, nDelayLines, nDelayLines> couplingScalars;
    Eigen::Matrix2f couplingMatrix2D;
    
    std::array<Eigen::Matrix<cf, nDelayLines, nDelayLines>, firOrder + 1> PolyMat; //Filter matrix represented as polynomials
    Eigen::Matrix<cf, nDelayLines, firOrder + 1> prevDelayLineOutput;   //to store previous delay line outputs for filtering
    Eigen::PermutationMatrix<3,3> perm;     //shift columns of storage matrix to the right
    //FIRFilter** couplingFilters;    //2D array of coupling Filters
    
    int nGroup;
    int *nSize;
    bool isFilter;
    float couplingCoeff;
    const float PI = std::acos(-1);
    float beta;     //diffraction filter cutoff, ranges between 0 and pi/2
    std::complex<float> I;  //square root of negative 1
    std::complex<float>* coeffs;
    
    // intermediate filter calculations
    std::array<Eigen::Matrix<cf, nDelayLines, nDelayLines>, firOrder + 1> M_Block_time_PolyMat;

    // temporaries for process()
    Eigen::Matrix<cf, nDelayLines, 1> delayLineInput;
    Eigen::Matrix<cf, nDelayLines, 1> filterOutput;
};
