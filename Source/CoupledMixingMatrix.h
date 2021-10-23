/*
  ==============================================================================

    CoupledMixingMatrix.h
    Created: 23 Oct 2021 5:44:11pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include <Eigen/Dense>


class CoupledMixingMatrix{
public:
    CoupledMixingMatrix();
    ~CoupledMixingMatrix();
    
    void initialize(int nGrp, int nDel);
    void updateBlockMatrix(Eigen::MatrixXf M_block_new);
    void updateCouplingCoeff(float alpha);
    void updateCouplingFilters();
    Eigen::VectorXf process(Eigen::VectorXf delayLineOutput);
    
private:
    Eigen::MatrixXf M_block;
    Eigen::MatrixXf couplingScalars;
    Eigen::Matrix2f couplingMatrix2D;
    int nGroup, nDelayLines, nSize;
    bool isFilter;
    float couplingCoeff;
    const float PI = std::acos(-1);
    
    
};
